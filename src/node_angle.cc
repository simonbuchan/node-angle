#include "nan.h"

#include "EGLWindowObject.hh"

namespace node_angle {

NAN_METHOD(add) {
  int32_t res = 0;
  for (int i = 0; i < info.Length(); i++) {
    auto arg = Nan::To<int32_t>(info[i]);
    if (arg.IsNothing()) {
      return Nan::ThrowTypeError("args should be integers.");
    }
    res += arg.FromJust();
  }
  info.GetReturnValue().Set(res);
}

NAN_MODULE_INIT(InitAll) {
  NAN_EXPORT(target, add);

  // Passing target down to the next NAN_MODULE_INIT
  EGLWindowObject::Init(target);
}

NODE_MODULE(angle, InitAll)

}
