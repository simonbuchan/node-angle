#include "nan.h"

#include "v8-helpers.hh"
#include "EGLWindowObject.hh"

namespace node_angle {

static const auto constAttribute =
  static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete);

#define EXPORT_GL_CONSTANT(target, name)       \
do {                                           \
  Nan::HandleScope scope;                      \
  Nan::ForceSet(                               \
    target,                                    \
    Nan::New(#name).ToLocalChecked(),          \
    Nan::New(GL_##name),                       \
    constAttribute);                           \
} while (0)

#define ARG_JUST(index, type, name)               \
  auto name##Maybe = Nan::To<type>(info[index]);  \
  if (name##Maybe.IsNothing()) {                  \
    return Nan::ThrowTypeError("Arg '"#name"' must be type "#type); \
  }                                               \
  auto name = name##Maybe.FromJust();

#define ARG_LOCAL(index, type, name)              \
  auto name##Maybe = Nan::To<type>(info[index]);  \
  if (name##Maybe.IsEmpty()) {                    \
    return Nan::ThrowTypeError("Arg '"#name"' must be type "#type); \
  }                                               \
  auto name = name##Maybe.ToLocalChecked();

#define ARG_STRING(index, name)                   \
  ARG_LOCAL(index, v8::String, name##Local)       \
  auto name = V8StringToUTF8StdString(name##Local);

NAN_METHOD(createShader) {
  ARG_JUST(0, int32_t, type);
  auto result = glCreateShader(type);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(shaderSource) {
  ARG_JUST(0, int32_t, shader);
  ARG_STRING(1, source);

  auto sourceArray = { source.data() };
  glShaderSource(shader, std::size(sourceArray), std::data(sourceArray), nullptr);
}

NAN_METHOD(compileShader) {
  ARG_JUST(0, int32_t, shader);
  glCompileShader(shader);
}

NAN_METHOD(deleteShader) {
  ARG_JUST(0, int32_t, shader);
  glDeleteShader(shader);
}

NAN_METHOD(getShaderParameter) {
  ARG_JUST(0, int32_t, shader);
  ARG_JUST(1, int32_t, pname);
  switch (pname) {
  default:
    return Nan::ThrowRangeError("Invalid shader parameter name");
  case GL_SHADER_TYPE:
  case GL_COMPILE_STATUS:
    break;
  }
  GLint value;
  glGetShaderiv(shader, pname, &value);
  info.GetReturnValue().Set(value);
}

NAN_METHOD(getShaderInfoLog) {
  ARG_JUST(0, int32_t, shader);
  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  if (length <= 1) {
    return info.GetReturnValue().SetEmptyString();
  }
  std::vector<GLchar> buffer(length);
  glGetShaderInfoLog(shader, length, nullptr, buffer.data());
  info.GetReturnValue().Set(Nan::New(buffer.data()).ToLocalChecked());
}

NAN_METHOD(createProgram) {
  auto result = glCreateProgram();
  info.GetReturnValue().Set(result);
}

NAN_METHOD(attachShader) {
  ARG_JUST(0, int32_t, program);
  ARG_JUST(1, int32_t, shader);
  glAttachShader(program, shader);
}

NAN_METHOD(linkProgram) {
  ARG_JUST(0, int32_t, program);
  glLinkProgram(program);
}

NAN_METHOD(deleteProgram) {
  ARG_JUST(0, int32_t, program);
  glDeleteProgram(program);
}

NAN_METHOD(useProgram) {
  ARG_JUST(0, int32_t, program);
  glUseProgram(program);
}

NAN_METHOD(getProgramParameter) {
  ARG_JUST(0, int32_t, program);
  ARG_JUST(1, int32_t, pname);
  switch (pname) {
  default:
    return Nan::ThrowRangeError("Invalid program parameter name");
  case GL_LINK_STATUS:
    break;
  }
  GLint value;
  glGetProgramiv(program, pname, &value);
  info.GetReturnValue().Set(value);
}

NAN_METHOD(getProgramInfoLog) {
  ARG_JUST(0, int32_t, program);
  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  if (length <= 1) {
    return info.GetReturnValue().SetEmptyString();
  }
  std::vector<GLchar> buffer(length);
  glGetProgramInfoLog(program, length, nullptr, buffer.data());
  info.GetReturnValue().Set(Nan::New(buffer.data()).ToLocalChecked());
}

NAN_METHOD(viewport) {
  ARG_JUST(0, int32_t, x);
  ARG_JUST(1, int32_t, y);
  ARG_JUST(2, int32_t, width);
  ARG_JUST(3, int32_t, height);
  glViewport(x, y, width, height);
}

NAN_METHOD(clearColor) {
  ARG_JUST(0, double, red);
  ARG_JUST(1, double, green);
  ARG_JUST(2, double, blue);
  ARG_JUST(3, double, alpha);
  glClearColor(
    static_cast<float>(red),
    static_cast<float>(green),
    static_cast<float>(blue),
    static_cast<float>(alpha));
}

NAN_METHOD(clear) {
  ARG_JUST(0, uint32_t, mask);
  glClear(mask);
}

NAN_METHOD(cullFace) {
  ARG_JUST(0, int32_t, mode);
  glCullFace(mode);
}

NAN_METHOD(enable) {
  ARG_JUST(0, int32_t, cap);
  glEnable(cap);
}

NAN_METHOD(disable) {
  ARG_JUST(0, int32_t, cap);
  glDisable(cap);
}

NAN_METHOD(createBuffer) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  info.GetReturnValue().Set(buffer);
}

NAN_METHOD(bindBuffer) {
  ARG_JUST(0, int32_t, target);
  ARG_JUST(1, uint32_t, buffer);
  glBindBuffer(target, buffer);
}

NAN_METHOD(bufferData) {
  ARG_JUST(0, int32_t, target);
  ARG_LOCAL(1, v8::Object, bufferData);
  ARG_JUST(2, int32_t, usage);

  Nan::TypedArrayContents<uint8_t> contents{ bufferData };
  if (!*contents) {
    return Nan::ThrowTypeError("Arg 'bufferData' must be any ArrayBufferView (e.g. Float32Array)");
  }

  glBufferData(target, contents.length(), *contents, usage);
}

NAN_METHOD(getAttribLocation) {
  ARG_JUST(0, int32_t, program);
  ARG_STRING(1, name);
  auto location = glGetAttribLocation(program, name.c_str());
  info.GetReturnValue().Set(location);
}

NAN_METHOD(uniformMatrix) {
  ARG_JUST(0, int32_t, location);
  ARG_JUST(1, int32_t, count);
  ARG_JUST(2, bool, transpose);
  ARG_LOCAL(3, v8::Object, matrix);

  if (!matrix->IsFloat32Array()) {
    return Nan::ThrowTypeError("Arg 'matrix' must be Float32Array.");
  }

  Nan::TypedArrayContents<float> contents{ matrix };
  if (contents.length() != 16) {
    return Nan::ThrowTypeError("Arg 'matrix' must have length 16.");
  }

  glUniformMatrix4fv(location, count, transpose, *contents);
}

NAN_METHOD(getUniformLocation) {
  ARG_JUST(0, int32_t, program);
  ARG_STRING(1, name);
  auto location = glGetUniformLocation(program, name.c_str());
  info.GetReturnValue().Set(location);
}

NAN_METHOD(enableVertexAttribArray) {
  ARG_JUST(0, int32_t, location);
  glEnableVertexAttribArray(location);
}

NAN_METHOD(vertexAttribPointer) {
  ARG_JUST(0, int32_t, location);
  ARG_JUST(1, int32_t, size);
  ARG_JUST(2, int32_t, type);
  ARG_JUST(3, bool, normalized);
  ARG_JUST(4, int32_t, stride);
  ARG_JUST(5, intptr_t, offset);
  glVertexAttribPointer(
    location,
    size,
    type,
    normalized,
    stride,
    reinterpret_cast<void*>(offset));
}

NAN_METHOD(drawArrays) {
  ARG_JUST(0, int32_t, mode);
  ARG_JUST(1, int32_t, first);
  ARG_JUST(2, int32_t, count);
  glDrawArrays(mode, first, count);
}

NAN_METHOD(drawElements) {
  ARG_JUST(0, int32_t, mode);
  ARG_JUST(1, int32_t, count);
  ARG_JUST(2, int32_t, type);
  ARG_JUST(3, intptr_t, offset);
  glDrawElements(mode, count, type, reinterpret_cast<void*>(offset));
}

NAN_MODULE_INIT(InitAll) {
  NAN_EXPORT(target, createShader);
  NAN_EXPORT(target, shaderSource);
  NAN_EXPORT(target, compileShader);
  NAN_EXPORT(target, deleteShader);
  NAN_EXPORT(target, getShaderParameter);
  NAN_EXPORT(target, getShaderInfoLog);

  NAN_EXPORT(target, createProgram);
  NAN_EXPORT(target, attachShader);
  NAN_EXPORT(target, linkProgram);
  NAN_EXPORT(target, deleteProgram);
  NAN_EXPORT(target, useProgram);
  NAN_EXPORT(target, getProgramParameter);
  NAN_EXPORT(target, getProgramInfoLog);

  NAN_EXPORT(target, viewport);
  NAN_EXPORT(target, clearColor);
  NAN_EXPORT(target, clear);
  NAN_EXPORT(target, cullFace);
  NAN_EXPORT(target, enable);
  NAN_EXPORT(target, disable);

  NAN_EXPORT(target, createBuffer);
  NAN_EXPORT(target, bindBuffer);
  NAN_EXPORT(target, bufferData);

  NAN_EXPORT(target, enableVertexAttribArray);
  NAN_EXPORT(target, vertexAttribPointer);
  NAN_EXPORT(target, getAttribLocation);
  NAN_EXPORT(target, uniformMatrix);
  NAN_EXPORT(target, getUniformLocation);

  NAN_EXPORT(target, drawArrays);
  NAN_EXPORT(target, drawElements);

  EXPORT_GL_CONSTANT(target, VERTEX_SHADER);
  EXPORT_GL_CONSTANT(target, FRAGMENT_SHADER);

  EXPORT_GL_CONSTANT(target, COMPILE_STATUS);
  EXPORT_GL_CONSTANT(target, LINK_STATUS);

  EXPORT_GL_CONSTANT(target, COLOR_BUFFER_BIT);

  EXPORT_GL_CONSTANT(target, ARRAY_BUFFER);
  EXPORT_GL_CONSTANT(target, ELEMENT_ARRAY_BUFFER);
  EXPORT_GL_CONSTANT(target, STATIC_DRAW);

  #undef FLOAT
  EXPORT_GL_CONSTANT(target, FLOAT);
  EXPORT_GL_CONSTANT(target, UNSIGNED_SHORT);

  EXPORT_GL_CONSTANT(target, TRIANGLES);

  EXPORT_GL_CONSTANT(target, BACK);
  EXPORT_GL_CONSTANT(target, CULL_FACE);

  EGLWindowObject::Init(target);
}

NODE_MODULE(angle, InitAll)

}
