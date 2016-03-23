// Extio include file.

#ifndef mainH 
#define mainH 

#include <stdio.h> 

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

#ifdef __cplusplus
} // __cplusplus defined.
#endif

#define null 0

#endif 