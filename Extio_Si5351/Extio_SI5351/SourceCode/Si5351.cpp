#include <windows.h> 
#include "lusb0_usb.h"
#include "extio_si5351.h" 
#include "SI5351.h"

extern long m_loFreq;
extern double m_referenceFrequency;
extern double m_correctionFactor;
extern unsigned char m_xtalLoad;

extern usb_dev_handle *handle;

struct PVALS {
	int p1;
	int p2;
	int p3;
};

// Static prototypes.
static char GetI2CStatus(usb_dev_handle *handle);
static int WriteI2CRegister(usb_dev_handle *, int, char);
static int ReadI2CRegister(usb_dev_handle *handle, char reg, char *value);
static int ReadI2C(usb_dev_handle *handle, char cmd, char *data, int responseLength);

static void clk0_set_clock_source(unsigned char clk_src);
static void clk1_set_clock_source(unsigned char clk_src);
static void clk2_set_clock_source(unsigned char clk_src);

static void clk0_reset();
static void clk1_reset();
static void clk2_reset();

static void clk0_powerup();
static void clk1_powerup();
static void clk2_powerup();

static void clk0_powerdown();
static void clk1_powerdown();
static void clk2_powerdown();

static void clk0_write_registers(struct PVALS pvals);
static void clk1_write_registers(struct PVALS pvals);
static void clk2_write_registers(struct PVALS pvals);

static void clk0_set_phase_offset(char offset);
static void clk1_set_phase_offset(char offset);
static void clk2_set_phase_offset(char offset);

static void plla_reset();
static void pllb_reset();

static void plla_write_registers(struct PVALS pvals);
static void pllb_write_registers(struct PVALS pvals);

static double corrected_xtal_freq(double xtal_freq);
static void clk_calc_pvals(double final_freq, double *pll_freq, struct PVALS *clk_pvals);
static void pll_calc_pvals(double pll_freq, double xtal_freq, struct PVALS *pll_pvals);

/// <summary>
/// Initialize the hardware.
/// </summary>
void DLL SI5351_Init(void)
{
    clk0_set_clock_source(SI5351_CLK_SRC_PLLA);
    clk1_set_clock_source(SI5351_CLK_SRC_PLLA);
    clk2_set_clock_source(SI5351_CLK_SRC_PLLB);

    clk0_reset();
    clk1_reset();
    clk2_reset();
        
    plla_reset();
    pllb_reset();
}

/// <summary>
/// Enable the output for a particular clock.
/// </summary>
/// <param name="clockNumber">Clock number.</param>
void DLL SI5351_OutputEnable(char clockNumber)
{
	if (clockNumber == SI5351_CLK_0)
		clk0_powerup();
	else if (clockNumber == SI5351_CLK_1)
		clk1_powerup(); 
	else if (clockNumber == SI5351_CLK_2)
		clk2_powerup();
}

/// <summary>
/// Disable the output for a particular clock.
/// </summary>
/// <param name="clockNumber">Clock number.</param>
void DLL SI5351_OutputDisable(char clockNumber)
{
	if (clockNumber == SI5351_CLK_0)
		clk0_powerdown();
	else if (clockNumber == SI5351_CLK_1)
		clk1_powerdown(); 
	else if (clockNumber == SI5351_CLK_2)
		clk2_powerdown();
}

/// <summary>
/// Set the clock frequency.
/// </summary>
/// <param name="freq">Clock frequency, in Hz.</param>
void DLL SI5351_clk0_set_freq(double frequencyHz)
{
	double pll_freq;
	struct PVALS clk_pvals;
	struct PVALS pll_pvals;

	// Initialize needed values.
	double final_freq = frequencyHz;
        
	// Calculate multisynth values for the final divider.
	clk_calc_pvals(final_freq, &pll_freq, &clk_pvals);
        
	// Calculate multisynth values for the pll.
	// This calculation takes the specified crystal correction
	// factor into account.
	double xtal_freq = corrected_xtal_freq(m_referenceFrequency);
	pll_calc_pvals(pll_freq, xtal_freq, &pll_pvals);  

	// Initialize the clock
	clk0_reset();
	clk1_reset();
        
	// Write the clock 0 registers.
	clk0_write_registers(clk_pvals);
	clk1_write_registers(clk_pvals);
	clk1_set_phase_offset((char)(pll_freq / final_freq));
        
	//Write the plla registers.
	plla_write_registers(pll_pvals);

	// Reset the pll
	plla_reset();
}

void DLL SI5351_clk2_set_freq(double frequencyHz)
{
	double pll_freq;
	struct PVALS clk_pvals;
	struct PVALS pll_pvals;

	// Initialize needed values.
	double final_freq = frequencyHz;
       
    // Calculate multisynth values for the final divider.
	clk_calc_pvals(final_freq, &pll_freq, &clk_pvals);
        
	// Calculate multisynth values for the pll.
	// This calculation takes the specified crystal correction
	// factor into account.
	double xtal_freq = corrected_xtal_freq(m_referenceFrequency);
	pll_calc_pvals(pll_freq, xtal_freq, &pll_pvals); 
        
    // Initialize the clock
    clk2_reset();
        
    // Write the clock 2 registers.
    clk2_write_registers(clk_pvals);
        
    // Write the pllb registers.
    pllb_write_registers(pll_pvals);
        
    // Reset the pll
    pllb_reset();
}

/*
Reset PLL
*/
static void plla_reset()
{
	while (WriteI2CRegister(handle, 177, (char)0x20) != 2);
}

static void pllb_reset()
{
	while (WriteI2CRegister(handle, 177, (char)0x80) != 2);
}   

/* 
Set CLK source.
*/
static void clk0_set_clock_source(unsigned char source)
{
	char value;
	while (ReadI2CRegister(handle, 16, &value) == 0);

    if (source == SI5351_CLK_SRC_PLLA)
		while (WriteI2CRegister(handle, 16, value & 0xdf) != 2);
	else
		while (WriteI2CRegister(handle, 16, value | 0x20) != 2);
}

static void clk1_set_clock_source(unsigned char source)
{
	char value;
	while (ReadI2CRegister(handle, 17, &value) == 0);

    if (source == SI5351_CLK_SRC_PLLA)
		while (WriteI2CRegister(handle, 17, value & 0xdf) != 2);
	else
		while (WriteI2CRegister(handle, 17, value | 0x20) != 2);
}

static void clk2_set_clock_source(unsigned char source)
{
	char value;
	while (ReadI2CRegister(handle, 18, &value) == 0);

    if (source == SI5351_CLK_SRC_PLLA)
		while (WriteI2CRegister(handle, 18, value & 0xdf) != 2);
	else
		while (WriteI2CRegister(handle, 18, value | 0x20) != 2);
}

/*
Reset CLK
*/
static void clk0_reset()
{
    // Power down the clock, then power it back up.
	char value;
	while (ReadI2CRegister(handle, 16, &value) == 0);
	while (WriteI2CRegister(handle, 16, value | 0x80) != 2);
	while (WriteI2CRegister(handle, 16, (value & 0x7f) | 0x0c) != 2);
}

static void clk1_reset()
{
    // Power down the clock, then power it back up.
	char value;
	while (ReadI2CRegister(handle, 17, &value) == 0);
	while (WriteI2CRegister(handle, 17, value | 0x80) != 2);
	while (WriteI2CRegister(handle, 17, (value & 0x7f) | 0x0c) != 2);
}

static void clk2_reset()
{
    // Power down the clock, then power it back up.
	char value;
	while (ReadI2CRegister(handle, 18, &value) == 0);
	while (WriteI2CRegister(handle, 18, value | 0x80) != 2);
	while (WriteI2CRegister(handle, 18, (value & 0x7f) | 0x0c) != 2);
}

/*
Power CLK up.
*/
static void clk0_powerup()
{
	char value;
    while (ReadI2CRegister(handle, 16, &value) == 0);
	while (WriteI2CRegister(handle, 16, value & 0x7f) != 2);
}

static void clk1_powerup()
{
	char value;
    while (ReadI2CRegister(handle, 17, &value) == 0);
	while (WriteI2CRegister(handle, 17, value & 0x7f) != 2);
}

static void clk2_powerup()
{
	char value;
    while (ReadI2CRegister(handle, 18, &value) == 0);
	while (WriteI2CRegister(handle, 18, value & 0x7f) != 2);
}

/*
Power CLK down.
*/
static void clk0_powerdown()
{
	char value;
    while (ReadI2CRegister(handle, 16, &value) == 0);
	while (WriteI2CRegister(handle, 16, value | 0x80) != 2);
}

static void clk1_powerdown()
{
	char value;
    while (ReadI2CRegister(handle, 17, &value) == 0);
	while (WriteI2CRegister(handle, 17, value | 0x80) != 2);
}

static void clk2_powerdown()
{
	char value;
    while (ReadI2CRegister(handle, 18, &value) == 0);
	while (WriteI2CRegister(handle, 18, value | 0x80) != 2);
}

/*
Clock phase offset.
*/
static void clk0_set_phase_offset(char offset)
{
    while (WriteI2CRegister(handle, 165, offset) != 2);
}

static void clk1_set_phase_offset(char offset)
{
    while (WriteI2CRegister(handle, 166, offset) != 2);
}

static void clk2_set_phase_offset(char offset)
{
    while (WriteI2CRegister(handle, 167, offset) != 2);
}

/*
Clock drive strength
*/
static void clk0_set_clock_drive(char clk_drive)
{
	char value;
    while (ReadI2CRegister(handle, 16, &value) == 0);
    while (WriteI2CRegister(handle, 16, (value & 0xfc) | (clk_drive & 0x03)) != 2);
}

static void clk1_set_clock_drive(char clk_drive)
{
	char value;
    while (ReadI2CRegister(handle, 17, &value) == 0);
    while (WriteI2CRegister(handle, 17, (value & 0xfc) | (clk_drive & 0x03)) != 2);
}

static void clk2_set_clock_drive(char clk_drive)
{
	char value;
    while (ReadI2CRegister(handle, 18, &value) == 0);
    while (WriteI2CRegister(handle, 18, (value & 0xfc) | (clk_drive & 0x03)) != 2);
}

/*
Clock write registers
*/
static void clk0_write_registers(struct PVALS pvals)
{
	while (WriteI2CRegister(handle, 42, (pvals.p3 >> 8) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 43, (pvals.p3 >> 0) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 44, (pvals.p1 >> 16) & 0x0003) != 2);
	while (WriteI2CRegister(handle, 45, (pvals.p1 >> 8) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 46, (pvals.p1 >> 0) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 47, ((pvals.p3 >> 12) & 0x00F0) | ((pvals.p2 >> 16) & 0x000F)) != 2);
	while (WriteI2CRegister(handle, 48, (pvals.p2 >> 8) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 49, (pvals.p2 >> 0) & 0x00FF) != 2);
}

static void clk1_write_registers(struct PVALS pvals)
{
    while (WriteI2CRegister(handle, 50, (pvals.p3 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 51, (pvals.p3 >> 0) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 52, (pvals.p1 >> 16) & 0x0003) != 2);
    while (WriteI2CRegister(handle, 53, (pvals.p1 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 54, (pvals.p1 >> 0) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 55, ((pvals.p3 >> 12) & 0x00F0) | ((pvals.p2 >> 16) & 0x000F)) != 2);
    while (WriteI2CRegister(handle, 56, (pvals.p2 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 57, (pvals.p2 >> 0) & 0x00FF) != 2); 
}

static void clk2_write_registers(struct PVALS pvals)
{
	while (WriteI2CRegister(handle, 58, (pvals.p3 >> 8) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 59, (pvals.p3 >> 0) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 60, (pvals.p1 >> 16) & 0x0003) != 2);
	while (WriteI2CRegister(handle, 61, (pvals.p1 >> 8) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 62, (pvals.p1 >> 0) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 63, ((pvals.p3 >> 12) & 0x00F0) | ((pvals.p2 >> 16) & 0x000F)) != 2);
	while (WriteI2CRegister(handle, 64, (pvals.p2 >> 8) & 0x00FF) != 2);
	while (WriteI2CRegister(handle, 65, (pvals.p2 >> 0) & 0x00FF) != 2);
}

/*
PLL write registers.
*/
static void plla_write_registers(struct PVALS pvals)
{
    while (WriteI2CRegister(handle, 26, (pvals.p3 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 27, (pvals.p3 >> 0) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 28, (pvals.p1 >> 16) & 0x0003) != 2);
    while (WriteI2CRegister(handle, 29, (pvals.p1 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 30, (pvals.p1 >> 0) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 31, ((pvals.p3 >> 12) & 0x00F0) | ((pvals.p2 >> 16) & 0x000F)) != 2);
    while (WriteI2CRegister(handle, 32, (pvals.p2 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 33, (pvals.p2 >> 0) & 0x00FF) != 2);    
}

static void pllb_write_registers(struct PVALS pvals)
{
    while (WriteI2CRegister(handle, 34, (pvals.p3 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 35, (pvals.p3 >> 0) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 36, (pvals.p1 >> 16) & 0x0003) != 2);
    while (WriteI2CRegister(handle, 37, (pvals.p1 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 38, (pvals.p1 >> 0) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 39, ((pvals.p3 >> 12) & 0x00F0) | ((pvals.p2 >> 16) & 0x000F)) != 2);
    while (WriteI2CRegister(handle, 40, (pvals.p2 >> 8) & 0x00FF) != 2);
    while (WriteI2CRegister(handle, 41, (pvals.p2 >> 0) & 0x00FF) != 2);
}

/*
Calculate clock p values.
*/
static void clk_calc_pvals(double final_freq, double *pll_freq, struct PVALS *clk_pvals)
{
	// Calculate the maximum pll frequency that can be used 
    // consistent with its frequency range.
	double pll_div = (final_freq < 150000000.0) ?
        (double)(int)(900000000.0 / final_freq) : 4.0;
            
    if (pll_div > 127.0)
        pll_div = 127.0;
    (*pll_freq) = final_freq * pll_div;
        
    // Calculate multisynth values for the final divider.
    (*clk_pvals).p1 = 128 * int(pll_div) - 512;
    (*clk_pvals).p2 = 0;
    (*clk_pvals).p3 = 1;
}

/*
Calculate pll p values
The xtal_freq needs to be the real crystal frequency,
including any correction factor.
*/
static void pll_calc_pvals(double pll_freq, double xtal_freq, struct PVALS *pll_pvals)
{        
	int a = (int)(pll_freq / xtal_freq);
	double b_over_c = (pll_freq / xtal_freq) - a;
	int c = (1 << 20) - 1;
	int b = (int)(b_over_c * c);
        
	(*pll_pvals).p1 = 128 * a + (int)(128 * (double)b / (double)c) - 512;
	(*pll_pvals).p2 = 128 * b - c * int(128 * (double)b / (double)c);
	(*pll_pvals).p3 = c;
}

/* 
Calculate a xtal frequency that takes the
correction factor into account.
*/
static double corrected_xtal_freq(double xtal_freq)
{
	return xtal_freq * (1.0 + m_correctionFactor / 10000000.0);
}

/*
Write a value to a register.
Returns number of bytes transferred.  Error if length transferred < 0.
*/
static int WriteI2CRegister(usb_dev_handle *handle, int reg, char value)
{
	char writeBuffer[] = {reg, value};
	int lengthTransferred = usb_control_msg(handle, 
		USB_CTRL_OUT, CMD_I2C_BEGIN | CMD_I2C_IO | CMD_I2C_END,
		I2C_M_WR, I2C_DEVICE_ADDRESS, writeBuffer, 2, 1000);
	return lengthTransferred;
}

/*
Read a value from a register.
*/
static int ReadI2CRegister(usb_dev_handle *handle, char reg, char *value)
{
	char status;
    int lengthTransferred = 0;
    int request = CMD_I2C_BEGIN | CMD_I2C_IO;

	// Send the device address.
	lengthTransferred = usb_control_msg(handle, 
		USB_CTRL_OUT, request, I2C_M_WR, 
		I2C_DEVICE_ADDRESS, &reg, 1, 1000);

    // Check the status.
    if (lengthTransferred == 1)
	{
		status = GetI2CStatus(handle);
		if (status != STATUS_ADDRESS_ACK)
			lengthTransferred = 0;
	}

	// Read the response.
	if (lengthTransferred == 1)
	{
		*value = 0;
		lengthTransferred = usb_control_msg(handle, 
			USB_CTRL_IN, CMD_I2C_IO + CMD_I2C_END, I2C_M_RD,
			I2C_DEVICE_ADDRESS, value, 1, 1000);
	}

	// Check the status.
	if (lengthTransferred == 1)
	{
		status = GetI2CStatus(handle);
		if (status != STATUS_ADDRESS_ACK)
			lengthTransferred = 0;
	}
    return lengthTransferred;
}

/*
Get the value of the current status bit.
*/
static char GetI2CStatus(usb_dev_handle *handle)
{
    // Get the current bus status.
    char status = 0;
    int lengthTransferred = ReadI2C(handle, CMD_GET_STATUS, &status, 1);
	return status;
}

/*
Issue a command to the i2c_tiny_usb interface and read the response.
This does not do anything on the I2C bus.  It is strictly a command issued
to the interface.
*/
static int ReadI2C(usb_dev_handle *handle, char cmd, char *data, int responseLength)
{
    int lengthTransferred = usb_control_msg(handle, 
		USB_CTRL_IN,cmd, I2C_M_WR,
		I2C_DEVICE_ADDRESS, data, responseLength, 1000);
	return lengthTransferred;
}