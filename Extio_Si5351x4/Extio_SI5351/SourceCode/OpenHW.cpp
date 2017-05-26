#include <windows.h> 
#include "lusb0_usb.h"
#include "extio_si5351x4.h" 
#include "SI5351x4.h"

extern long m_loFreq;
extern int m_referenceFrequency;
extern unsigned int m_clockOutputSetting;
extern usb_dev_handle *handle;
extern bool m_outputEnabled;


/*
This entry is called each time the user specifies that the audio data input should be received
through the hardware managed by this DLL or, if still using the sound card for this, that the 
DLL must activate control of the external hardware. It can be used by the DLL itself for delayed
init tasks, like, e.g., the display of its own GUI, if the DLL has one. 
*/
bool DLL OpenHW(void) 
{ 
	// Find the busses and devices on the USB connection.
    usb_init();
    usb_find_busses();
    usb_find_devices();

	// Find the usb device and open it.
	handle = OpenUsbDevice(I2C_TINY_USB_VID, I2C_TINY_USB_PID);
	if (handle != null)
	{
		SI5351_Init();
        SI5351_OutputDisable(0);
        SI5351_OutputDisable(1);
		m_outputEnabled = false;
	}
	return (handle != null); 
} 


/*
Open the USB device and return a handle.
Return null if device cannot be opened.
*/
usb_dev_handle *OpenUsbDevice(int vid, int pid)
{
	struct usb_bus *bus;
    struct usb_device *dev;

	// For each of the USB busses...
	handle = null;
    for (bus = usb_get_busses(); bus; bus = bus->next) 
    {
		// For each device on the bus...
        for(dev = bus->devices; dev; dev = dev->next) 
        {
			// Is this the device we want?
            if((dev->descriptor.idVendor == I2C_TINY_USB_VID) && 
                (dev->descriptor.idProduct == I2C_TINY_USB_PID)) 
            {
                /* Open the device */
                handle = usb_open(dev);
                break;
            }
        }
    }
	return handle;
}

