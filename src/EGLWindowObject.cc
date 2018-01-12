#include "EGLWindowObject.hh"
#include "v8-helpers.hh"

namespace node_angle {

  NAN_MODULE_INIT(EGLWindowObject::Init) {
    auto tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Window").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "swapBuffers", swapBuffers);

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

METHOD(swapBuffers) {
  Unwrap<EGLWindowObject>(info.This())->eglWindow_->swap();
}

METHOD(New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowError("gl.Window must be called with new.");
  }

  auto args = !info.Length()
    ? Nan::New<v8::Object>()
    : Nan::To<v8::Object>(info[0]);
  auto titleLocal = ToLocalOrDefault(
    MaybeGet(args, "title"),
    Nan::New("ANGLE Window").ToLocalChecked());
  auto width = ToJustOrDefault(MaybeGet(args, "width"), 1280);
  auto height = ToJustOrDefault(MaybeGet(args, "height"), 720);

  auto osWindow = std::unique_ptr<OSWindow>(CreateOSWindow());

  auto title = V8StringToUTF8StdString(titleLocal);

  if (!osWindow->initialize(title, width, height)) {
    return Nan::ThrowError("Failed to initialize OS window.");
  }

  auto glesMajorVersion = 2;
  auto glesMinorVersion = 0;

  auto eglWindow = std::make_unique<EGLWindow>(
    glesMajorVersion,
    glesMinorVersion,
    EGLPlatformParameters{});

  eglWindow->setDebugEnabled(true);
  eglWindow->setDebugLayersEnabled(true);
  eglWindow->setConfigRedBits(8);
  eglWindow->setConfigGreenBits(8);
  eglWindow->setConfigBlueBits(8);
  eglWindow->setConfigAlphaBits(8);
  eglWindow->setConfigDepthBits(24);
  eglWindow->setConfigStencilBits(8);
  eglWindow->setWebGLCompatibilityEnabled(true);

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

void emit(v8::Local<v8::Object> self, const char* name, std::initializer_list<v8::Local<v8::Value>> args)
{
  std::vector<v8::Local<v8::Value>> emitArgs;
  emitArgs.push_back(Nan::New(name).ToLocalChecked());
  std::copy(std::begin(args), std::end(args), std::back_inserter(emitArgs));
  Nan::MakeCallback(self, "emit", std::size(emitArgs), std::data(emitArgs));
}

void EGLWindowObject::idle_impl() {
  assert(!closed_);

  osWindow_->messageLoop();

  bool resized = false;

  Event event;
  while (osWindow_->popEvent(&event)) {
    switch (event.Type) {
    case Event::EVENT_RESIZED: resized = true; break; // FIXME: messageLoop() queues these resizes up, which is totally pointless!
    case Event::EVENT_CLOSED: closed_ = true; break;
    }
  }

  if (!closed_) {
    Nan::HandleScope scope;
    if (resized) {
      auto width = Nan::New(osWindow_->getWidth());
      auto height = Nan::New(osWindow_->getHeight());

      auto widthName = Nan::New("width").ToLocalChecked();
      auto heightName = Nan::New("height").ToLocalChecked();

      Nan::Set(handle(), widthName, width);
      Nan::Set(handle(), heightName, height);
      emit(handle(), "resized", { width, height });
    }

    emit(handle(), "preswap", {});
    eglWindow_->swap();
    emit(handle(), "postswap", {});
  }

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
