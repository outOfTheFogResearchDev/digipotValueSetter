#include <node.h>

using namespace v8;

void Connect(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  long devices = GetDevices()
  args.GetReturnValue().Set(input);
}

void Init(Local<Object> exports, Local<Object> method)
{
  NODE_SET_METHOD(method, "exports", Connect);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init);