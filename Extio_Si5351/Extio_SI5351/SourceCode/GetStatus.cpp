#include <windows.h> 
#include "extio_si5351.h" 

extern long m_loFreq;

/*
This entry point is meant to allow the DLL to return status information upon request. 
Presently it is never called, though its existence is checked when the DLL is loaded
so it must implemented, even if in a dummy way. It is meant for future expansions, 
for complex HW that implement e.g. a preselector or some other controls other than a 
simple LO frequency selection. 
*/
int DLL GetStatus(void) 
{
	return 0;
}
 
