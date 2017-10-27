namespace node_angle {

inline auto MaybeGet(Nan::MaybeLocal<v8::Object> obj, const char* name) {
  if (obj.IsEmpty()) {
    return Nan::MaybeLocal<v8::Value>();
  }
  auto key = Nan::New(name).ToLocalChecked().As<v8::Value>();
  return Nan::Get(obj.ToLocalChecked(), key);
}

template <typename T>
inline auto ToJustOrDefault(Nan::MaybeLocal<v8::Value> value, T defaultValue) -> T {
  if (value.IsEmpty()) {
    return defaultValue;
  }
  auto result = Nan::To<T>(value.ToLocalChecked());
  if (result.IsNothing()) {
    return defaultValue;
  }
  return result.FromJust();
}

template <typename T>
inline auto ToLocalOrDefault(Nan::MaybeLocal<v8::Value> value, v8::Local<T> defaultValue)
-> v8::Local<T> {
  if (value.IsEmpty()) {
    return defaultValue;
  }
  auto result = Nan::To<T>(value.ToLocalChecked());
  if (result.IsEmpty()) {
    return defaultValue;
  }
  return result.ToLocalChecked();
}

inline auto V8StringToUTF8StdString(v8::Local<v8::String> value) {
  auto length = value->Utf8Length();
  std::string result(length, '\0');
  value->WriteUtf8(&result[0]);
  return result;
}

}