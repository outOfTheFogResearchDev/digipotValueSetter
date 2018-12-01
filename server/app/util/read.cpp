#include <node.h>
#include "StdAfx.h"
#include "VCAIOUSB.h"
#include <math.h>
#include <ctype.h>
#include <time.h>

using namespace v8;

void Read(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  unsigned long devicesMask = GetDevices();
  Local<Array> devices = Array::New(isolate);
  for (unsigned int i = 0; i < 8; i++) {
      devices->Set(Number::New(isolate, i), Boolean::New(isolate, false));
      if (devicesMask & (1 << i)) devices->Set(Number::New(isolate, i), Boolean::New(isolate, true));
  }
  args.GetReturnValue().Set(devices);
}

void Init(Local<Object> exports, Local<Object> method)
{
  NODE_SET_METHOD(method, "exports", Read);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init);