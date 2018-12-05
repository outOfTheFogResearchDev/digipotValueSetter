#include <node.h>
#include "StdAfx.h"
#include "VCAIOUSB64.h"

using namespace v8;

void configure(const FunctionCallbackInfo<Value> &args)
{

    //* C++ starts here

    unsigned long outMask = 0x07, DIOdata = 0xFFFFFFFF;
    // Assume that we are only using one ACCES I/O device in the USB chain.
    unsigned long deviceIndex = diOnly;

    // Configure device for writing to first 3 DIO ports (Ports A-C write, Port D read).
    DIO_Configure(deviceIndex, false, &outMask, &DIOdata);

    //* C++ ends here
}

void init(Local<Object> exports, Local<Object> method)
{
    NODE_SET_METHOD(method, "exports", configure);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init);