#include <windows.h> 
#include "extio.h" 


// Global variables.
long m_loFreq; 
long m_referenceFrequency;

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

		// .......... init here the hardware controlled by the DLL 

		//  ......... init here the DLL graphical interface, if any ........ 

		first = false;  
	}	 

	// Return data via the sound card.
	type = 4;

	// Set the name and model.
	strcpy(name, "");
	strcpy(model, "");
 
	return true; 
} 