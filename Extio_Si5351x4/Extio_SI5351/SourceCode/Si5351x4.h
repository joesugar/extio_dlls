#ifndef _SI_5351_H_
#define _SI_5351_H_

#include <windows.h> 
#include "lusb0_usb.h"
#include "extio_si5351x4.h" 

#define I2C_DEVICE_ADDRESS	0x60

// Si5351 definitions.
#define SI5351_CLK_0           0
#define SI5351_CLK_1           1
#define SI5351_CLK_2           2

// Clock sources.
#define SI5351_CLK_SRC_PLLA    0
#define SI5351_CLK_SRC_PLLB    1
        
// Clock drive.
#define SI5351_CLK_DRIVE_2MA   0
#define SI5351_CLK_DRIVE_4MA   1
#define SI5351_CLK_DRIVE_6MA   2
#define SI5351_CLK_DRIVE_8MA   3
        
// Xtal load capacitance
#define SI5351_XTAL_LOAD_6PF   0x52
#define SI5351_XTAL_LOAD_8PF   0x92     
#define SI5351_XTAL_LOAD_10PF  0xd2

#define SI5351_XTAL_FREQUENCY  25000000.0
#define SI5351_XTAL_CORRECTION -1382.7

// USB contants used to build the request type for a control transfer.
#ifndef USB_ENDPOINT_OUT
#define USB_ENDPOINT_OUT		(0 << 7)
#endif
#ifndef USB_ENDPOINT_IN
#define USB_ENDPOINT_IN			(1 << 7)
#endif

#ifndef USB_TYPE_STANDARD
#define USB_TYPE_STANDARD		(0 << 5)
#endif
#ifndef USB_TYPE_CLASS
#define USB_TYPE_CLASS			(1 << 5)
#endif
#ifndef USB_TYPE_VENDOR
#define USB_TYPE_VENDOR			(2 << 5)
#endif
#ifndef USB_TYPE_RESERVED
#define USB_TYPE_RESERVED		(3 << 5)
#endif

#define USB_RECIPIENT_DEVICE	0
#define USB_RECIPIENT_INTERFACE 1
#define USB_RECIPIENT_ENDPOINT  2
#define USB_RECIPIENT_OTHER		3

#define USB_CTRL_OUT	USB_ENDPOINT_OUT | USB_TYPE_CLASS | USB_RECIPIENT_DEVICE
#define USB_CTRL_IN		USB_ENDPOINT_IN | USB_TYPE_CLASS | USB_RECIPIENT_DEVICE

// I2C read/write flags.
#define I2C_M_WR	0x00
#define I2C_M_RD	0x01

// I2C commands.
#define CMD_I2C_BEGIN	1
#define CMD_I2C_END		2
#define CMD_I2C_IO		4
#define CMD_I2C_PROBE	CMD_I2C_BEGIN | CMD_I2C_END

// USB interface commands -- must match command ids in firmware.
#define CMD_ECHO		8
#define CMD_GET_FUNC	9
#define CMD_SET_DELAY	10
#define CMD_GET_STATUS	11

// USB status flags.
#define STATUS_IDLE			0
#define STATUS_ADDRESS_ACK  1
#define STATUS_ADDRESS_NAK  2

// Make sure functions are exported with C linkage under C++ compilers.
#ifdef __cplusplus
extern "C"
{
#endif

void DLL SI5351_Init(void);
void DLL SI5351_OutputEnable(char clockNumber);
void DLL SI5351_OutputDisable(char clockNumber);
void DLL SI5351_clk0_set_freq(double frequencyHz);
void DLL SI5351_clk1_set_freq(double frequencyHz);
void DLL SI5351_clk2_set_freq(double frequencyHz);

#ifdef __cplusplus
} // __cplusplus defined.
#endif

#endif