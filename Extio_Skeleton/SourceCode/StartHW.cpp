#include <windows.h> 
#include "extio.h" 

extern long m_loFreq;

/*
 * This entry is called each time the user presses the Start button on the main screen
 * after having previously specified that the DLL is in control of the external hardware. 
 */
int DLL StartHW(long freq) 
{ 
	// Set the frequency of the controlled hardware to LOfreq 

    // Return the number of complex elements to be returned on each
	// invocation of the callback routine.
	return 0;
} 