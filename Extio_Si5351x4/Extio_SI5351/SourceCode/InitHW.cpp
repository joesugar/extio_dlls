#include <windows.h> 
#include "lusb0_usb.h"
#include "extio_si5351x4.h" 
#include "SI5351x4.h"

// Global variables.
long m_loFreq; 
double m_referenceFrequency;
double m_correctionFactor;
unsigned char m_xtalLoad;
bool m_outputEnabled = false;

usb_dev_handle *handle = NULL;

/*  
This function is the first called at startup and is used both to tell the DLL that it is
time to initialize the hardware, and to get back a descriptive name and model (or Serial 
Number) of the HW, together with a type code. 

The type code is an index code used to identify the hardware type supported by the  DLL. 
Please use one the following values : 
 
3 	The hardware does its own digitization and the audio data are returned
	via the callback device. Data must be in 16‐bit  (short) format, little endian. 
4 	The audio data are returned via the sound card. The external hardware just 
	controls the LO, and possibly a preselector, under DLL control. 
5 	The hardware does its own digitization and the audio data are returned
	via the callback device. Data are in 24‐bit integer format, little endian. 
6 	The hardware does its own digitization and the audio data are returned
	via the callback device. Data are in 32‐bit integer format, little endian. 
7 	The hardware does its own digitization and the audio data are returned
	via the callback device. Data are in 32‐bit float format, little endian. 
*/

bool DLL InitHW(char *name, char *model, int& type) 
{ 
	static bool first = true; 
 
	if(first) 
	{ 
		// Default local oscillator frequency, in Hz.
		m_loFreq = 10000000;
		m_referenceFrequency = SI5351_XTAL_FREQUENCY;
		m_correctionFactor = SI5351_XTAL_CORRECTION;
		m_xtalLoad = SI5351_XTAL_LOAD_8PF;
		m_outputEnabled = false;

		// .......... init here the hardware controlled by the DLL 
 
		//  ......... init here the DLL graphical interface, if any ........ 

		first = false;  
	}	 

	// Return data via the sound card.
	type = 4;

	// Set the name and model.
	strcpy(name, "SI5351 SDR");
	strcpy(model, "V1.0");
 
	return true; 
} 