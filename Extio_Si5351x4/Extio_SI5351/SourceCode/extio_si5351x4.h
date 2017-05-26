// Extio_SI5351 include file.

#ifndef mainH 
#define mainH 

#include <stdio.h> 
#include "lusb0_usb.h"

#define EXPORT

#ifdef _WIN32
	// Define calling convention in one place, for convenience.
	#define DLL _stdcall
#else
	#define DLL
#endif

// Make sure functions are exported with C linkage under C++ compilers.
#ifdef __cplusplus
extern "C"
{
#endif

// Function definitions.
void DLL CloseHW(void);
int  DLL GetStatus(void);
bool DLL InitHW(char *, char *, int&);
bool DLL OpenHW(void);
void DLL SetCallback(void (* Callback)(int, int, float, short *));
int  DLL SetHWLO(long);
int  DLL StartHW(long);
void DLL StopHW(void);

usb_dev_handle *OpenUsbDevice(int vid, int pid);

#ifdef __cplusplus
} // __cplusplus defined.
#endif

#define null 0

#define I2C_TINY_USB_VID 0x16c0
#define I2C_TINY_USB_PID 0x05dc

#endif 