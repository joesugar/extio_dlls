#include <windows.h> 
#include "extio.h" 

extern long m_loFreq;
extern int m_referenceFrequency;

/*
This entry point is used to communicate and control the desired frequency of
the external HW via the DLL. The frequency is expressed in units of Hz. The 
entry point is called at each change (done by any means) of the LO value in 
the main screen.  
*/
int DLL SetHWLO(long freq) 
{
	// Set here the LO frequency in the controlled hardware 
	// Return 0 if the frequency is within the limits the HW can generate.
	m_loFreq = freq;
	return 0;
}
