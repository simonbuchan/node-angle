#include "EGLWindowObject.hh"

namespace node_angle {

  NAN_MODULE_INIT(EGLWindowObject::Init) {
    auto tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Window").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::Set(
      target,
      Nan::New("Window").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

  void EGLWindowObject::uv_idle(uv_idle_t* idle) {
    auto obj = static_cast<EGLWindowObject*>(idle->data);

    obj->idle_impl();
  }

  EGLWindowObject::EGLWindowObject(
    std::unique_ptr<OSWindow> osWindow
    , std::unique_ptr<EGLWindow> eglWindow
  )
    : osWindow_{ std::move(osWindow) }
    , eglWindow_{ std::move(eglWindow) }
    , closed_{ false }
{
}

#define METHOD(name) NAN_METHOD(EGLWindowObject::name)

METHOD(New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowError("EGLWindow must be called with new.");
  }

  auto osWindow = std::unique_ptr<OSWindow>(CreateOSWindow());

  if (!osWindow->initialize("My first EGLWindow!", 1280, 720)) {
    return Nan::ThrowError("Failed to initialize OS window.");
  }

  auto glesMajorVersion = 2;
  auto glesMinorVersion = 0;

  auto eglWindow = std::make_unique<EGLWindow>(
    glesMajorVersion,
    glesMinorVersion,
    EGLPlatformParameters{});

  eglWindow->setConfigRedBits(8);
  eglWindow->setConfigGreenBits(8);
  eglWindow->setConfigBlueBits(8);
  eglWindow->setConfigAlphaBits(8);
  eglWindow->setConfigDepthBits(24);
  eglWindow->setConfigStencilBits(8);

  if (!eglWindow->initializeGL(osWindow.get())) {
    return Nan::ThrowError("Failed to initialize EGL.");
  }

  auto obj = new EGLWindowObject(
    std::move(osWindow),
    std::move(eglWindow));

  obj->Wrap(info.This());

  if (auto err = uv_idle_init(uv_default_loop(), &obj->uv_idle_)) {
    return Nan::ThrowError(uv_strerror(err));
  }

  obj->uv_idle_.data = obj;

  if (auto err = uv_idle_start(&obj->uv_idle_, uv_idle)) {
    return Nan::ThrowError(uv_strerror(err));
  }

  obj->osWindow_->setVisible(true);

  info.GetReturnValue().Set(info.This());
}

METHOD(Close) {
  osWindow_->pushEvent(Event{ Event::EVENT_CLOSED });
}

METHOD(Destroy) {
  if (!closed_) {
    destroy_impl();
  }
}

void EGLWindowObject::idle_impl() {
  assert(!closed_);

  Event event;
  while (osWindow_->popEvent(&event)) {
    // TODO: dispatch node events
    switch (event.Type) {
    case Event::EVENT_CLOSED:
      closed_ = true;
      break;
    }
  }

  osWindow_->messageLoop();

  if (closed_) {
    destroy_impl();
  }
}

void EGLWindowObject::destroy_impl() {
  eglWindow_->destroyGL();
  osWindow_->destroy();
  uv_idle_stop(&uv_idle_);
}

}
