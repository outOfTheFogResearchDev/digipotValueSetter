// AIOUSB.h : Header file for AIOUSB.dll
//
//For Microsoft Visual C++, use VCAIOUSB.lib
//For Borland C++ Builder, use CBAIOUSB.lib

#ifdef __cplusplus
extern "C" {
#endif

#define diOnly 0xFFFFFFFD
#define diFirst 0xFFFFFFFE


__declspec(dllimport) unsigned long GetDevices(void);
__declspec(dllimport) unsigned long QueryDeviceInfo(unsigned long DeviceIndex, unsigned long *pPID, unsigned long *pNameSize, char *pName, unsigned long *pDIOBytes, unsigned long *pCounters);

__declspec(dllimport) unsigned long DIO_Configure(unsigned long DeviceIndex, unsigned char bTristate, void *pOutMask, void *pData);

__declspec(dllimport) unsigned long DIO_Write1(unsigned long DeviceIndex, unsigned long BitIndex, unsigned char bData);
__declspec(dllimport) unsigned long DIO_Write8(unsigned long DeviceIndex, unsigned long ByteIndex, unsigned char Data);
__declspec(dllimport) unsigned long DIO_WriteAll(unsigned long DeviceIndex, void *pData);

__declspec(dllimport) unsigned long DIO_Read8(unsigned long DeviceIndex, unsigned long ByteIndex, unsigned char *pBuffer);
__declspec(dllimport) unsigned long DIO_ReadAll(unsigned long DeviceIndex, void *Buffer);

__declspec(dllimport) unsigned long CTR_8254Mode(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned long Mode);
__declspec(dllimport) unsigned long CTR_8254ModeLoad(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned long Mode, unsigned short LoadValue);
__declspec(dllimport) unsigned long CTR_8254ReadModeLoad(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned long Mode, unsigned short LoadValue, unsigned short *pReadValue);
__declspec(dllimport) unsigned long CTR_8254Read(unsigned long DeviceIndex, unsigned long BlockIndex, unsigned long CounterIndex, unsigned short *pReadValue);

__declspec(dllimport) unsigned long CTR_StartOutputFreq(unsigned long DeviceIndex, unsigned long CounterIndex, double *pHz);

__declspec (dllimport) unsigned long DACOutputOpen(unsigned long DeviceIndex, double *ClockHz);
__declspec (dllimport) unsigned long DACOutputClose(unsigned long DeviceIndex, unsigned long bWait);
__declspec (dllimport) unsigned long DACOutputFrame(unsigned long DeviceIndex, unsigned long FramePoints, unsigned short *FrameData);
__declspec (dllimport) unsigned long DACDirect(unsigned long DeviceIndex, unsigned long Channel, unsigned short Counts);
__declspec (dllimport) unsigned long DACMultiDirect(unsigned long DeviceIndex, void *pDACData, unsigned long DACDataCount);

__declspec (dllimport) unsigned long CustomEEPROMRead(unsigned long DeviceIndex, unsigned long StartAddress, unsigned long *DataSize, void *pData);
__declspec (dllimport) unsigned long CustomEEPROMWrite(unsigned long DeviceIndex, unsigned long StartAddress, unsigned long DataSize, void *pData);

#ifdef __cplusplus
}
#endif
