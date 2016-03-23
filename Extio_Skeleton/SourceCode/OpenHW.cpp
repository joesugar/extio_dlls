#include <windows.h> 
#include "extio.h" 

/*
 * This entry is called each time the user specifies that the audio data input should be
 * received through the hardware managed by this DLL or, if still using the sound card
 * for this, that the DLL must activate control of the external hardware. It can be used
 * by the DLL itself for delayed init tasks, like, e.g., the display of its own GUI, if 
 * the DLL has one. 
 */
bool DLL OpenHW(void) 
{ 
	return TRUE;
} 
