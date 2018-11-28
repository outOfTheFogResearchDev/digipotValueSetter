#include <node.h>

using namespace v8;

void Say(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  Local<String> input = args[0].As<String>();
  args.GetReturnValue().Set(input);
}

void Initialize(Local<Object> exports)
{
  NODE_SET_METHOD(exports, "say", Say);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);