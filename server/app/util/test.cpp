#include <napi.h>
using namespace Napi;

std::string Say(const CallbackInfo& info) {
  std::string input = info[0].As<String>().Utf8Value();
  return input;
}

Object Init(Env env, Napi::Object exports) {
  exports.Set("say", Function::New(env, Say));
  return exports;
}

NODE_API_MODULE(addon, Init)