#include <windows.h> 
#include "extio.h" 

extern long m_loFreq;

/*  
 * This entry is called when the user indicates that control of the external HW is 
 * no longer needed or wanted. This is done by choosing ShowOptions | Select Input 
 * then selecting either WAV file or Sound Card.  The  DLL  can  use  this  information 
 * to  e.g.  shut  down  its  GUI  interface,  if  any,  and  possibly  to  put  the 
 * controlled HW in a idle status. 
 */
void DLL CloseHW(void) 
{
	// Close the device if it's been opened.
}