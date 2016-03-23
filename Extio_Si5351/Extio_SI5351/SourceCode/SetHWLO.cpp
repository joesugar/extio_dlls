#include <windows.h> 
#include "extio_si5351.h" 
#include "SI5351.h"

extern long m_loFreq;
extern int m_referenceFrequency;
extern bool m_outputEnabled;

/*
This entry point is used to communicate and control the desired frequency of
the external HW via the DLL. The frequency is expressed in units of Hz. The 
entry point is called at each change (done by any means) of the LO value in 
the main screen.  
*/
int DLL SetHWLO(long freq) 
{
	// If the frequency is below 1.25 MHz return with no change because
	// the Si5351 doesn't work well below that frequency.  No change
	// indicated by returning 0.

	// Perform necessary calculations and set the frequency.
	m_loFreq = freq; 
	if (m_outputEnabled)
	{
		SI5351_OutputDisable(0);
		SI5351_OutputDisable(1);
	}

	// The set frequency routine contains a clock reset, which enables
	// the clock output.  Be sure to disable the clock on return if
	// that's the way you want it.
	SI5351_clk0_set_freq((double)freq);

	if (!m_outputEnabled)
	{
		SI5351_OutputDisable(0);
		SI5351_OutputDisable(1);
	}

	// Set here the LO frequency in the controlled hardware 
	return (int)freq; 
}
