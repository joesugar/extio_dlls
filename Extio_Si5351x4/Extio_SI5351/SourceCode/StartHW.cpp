#include <windows.h> 
#include "extio_si5351x4.h" 
#include "SI5351x4.h"

extern long m_loFreq;
extern bool m_outputEnabled;

/*
This entry is called each time the user presses the Start button on the main screen
after having previously specified that the DLL is in control of the external hardware. 
*/
int DLL StartHW(long freq) 
{ 
	// Set the frequency of the controlled hardware to LOfreq 
	// Normally you would enable the output here but if you set m_outputEnabled
	// to TRUE before setting the frequency it will be taken care of automatically.
	m_outputEnabled = true;
	SetHWLO(freq);
	//SI5351_OutputEnable(0);
	//SI5351_OutputEnable(1);


	// Return the number of complex elements to be returned on each
	// invocation of the callback routine.
	return 0;
} 