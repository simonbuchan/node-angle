#ifndef NODE_ANGLE_EGLWINDOWOBJECT_HH
#define NODE_ANGLE_EGLWINDOWOBJECT_HH 1

#include <memory>

#include "nan.h"
#include "EGLWindow.h"
#include "OSWindow.h"
#include "uv.h"


namespace node_angle {

struct EGLWindowObject : Nan::ObjectWrap {
  static NAN_MODULE_INIT(Init);

private:
  static void uv_idle(uv_idle_t*);

  explicit EGLWindowObject(
      std::unique_ptr<OSWindow> osWindow
    , std::unique_ptr<EGLWindow> eglWindow
  );
  ~EGLWindowObject() = default;

  std::unique_ptr<OSWindow> osWindow_;
  std::unique_ptr<EGLWindow> eglWindow_;
  uv_idle_t uv_idle_;
  bool closed_;

  static NAN_METHOD(swapBuffers);

  static NAN_METHOD(New);

  void idle_impl();
  void destroy_impl();
};

}

#endif // NODE_ANGLE_EGLWINDOWOBJECT_HH 1
