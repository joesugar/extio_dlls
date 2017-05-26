#include <windows.h> 
#include "extio_si5351x4.h" 
#include "SI5351x4.h"

extern long m_loFreq;

/* 
This entry is called each time the user presses the Stop button on the Winrad main screen. 
It can be used for whatever task might be needed in such an occurrence.  If the external 
HW does not provide the audio data, being, e.g., just a DDS or some other sort of an 
oscillator, typically this call is a No‐op. The DLL could also use this call to hide its 
GUI, if any.  If otherwise the external HW sends the audio data via the USB port, or any 
other hardware port managed by the DLL, when this entry is called, the HW should be 
commanded by the DLL to stop sending data. 
*/
void DLL StopHW(void)
{ 
	SI5351_OutputDisable(0);
	SI5351_OutputDisable(1);
	return;  // nothing to do with this specific HW 
} 