#include <windows.h> 
#include "lusb0_usb.h"
#include "extio_si5351x4.h" 
#include "SI5351x4.h"

extern long m_loFreq;
extern usb_dev_handle *handle;

/*  
This entry is called when the user indicates that control of the external HW is 
no longer needed or wanted. This is done by choosing ShowOptions | Select Input 
then selecting either WAV file or Sound Card.  The  DLL  can  use  this  information 
to  e.g.  shut  down  its  GUI  interface,  if  any,  and  possibly  to  put  the 
controlled HW in a idle status. 
*/
void DLL CloseHW(void) 
{
	// Close the device if it's been opened.
	if (handle != null)
	{
		SI5351_OutputDisable(0);
		SI5351_OutputDisable(1);
		usb_close(handle);
		handle = null;
	}
}