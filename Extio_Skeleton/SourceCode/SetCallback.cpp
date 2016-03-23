#include <windows.h> 
#include "extio.h" 

/*
 * This entry point is used by Winrad to communicate to the DLL the function 
 * address that it should invoke when a new buffer of audio data is ready, or 
 * when an  asynchronous event must be communicated by the DLL. Of course the 
 * new buffer of audio data  is only sent by DLLs that control HW that have 
 * their own  internal digitizers and do not depend on the soundcard for input. 
 * In this case it’s up to the DLL to decide which I/O port is used to read from 
 * the HW the digitized audio data stream. One  example is the USB port.  If
 * you don’t foresee the need of an asynchronous communication started from 
 * the DLL, simply do a return when Winrad calls this entry point. 
 */
void DLL SetCallback(void (* Callback)(int, int, float, short *)) 
{
	return;
}
