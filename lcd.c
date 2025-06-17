#include "lcd.h"
#include "I2C_bitbang.h" 

#define NULL 0

#define PULSE_PERIOD 500
#define CMD_PERIOD 5000

#define BACKLIGHT 8
#define DATA 1
static int iBackLight = BACKLIGHT;



void wait(unsigned int pulses){
	int i;
	
	for (i = 0; i < pulses; i++);
}


// Send a command to LCD
//
static void WriteCommand(unsigned char iAddr, unsigned char ucCMD)
{
unsigned char uc;
unsigned char rc;

	i2c_start();
	i2c_write(iAddr);
	rc = read_ack();
	
	uc = (ucCMD & 0xf0) | iBackLight; // most significant nibble sent first	
	i2c_write(uc);
	rc = read_ack();
	
	wait(PULSE_PERIOD); // manually pulse the clock line
	uc |= 4; // enable pulse
	i2c_write(uc);
	rc = read_ack();
	
	wait(PULSE_PERIOD);
	uc &= ~4; // toggle pulse
	i2c_write(uc);
	rc = read_ack();
	
	wait(CMD_PERIOD);
	uc = iBackLight | (ucCMD << 4); // least significant nibble
	i2c_write(uc);
	rc = read_ack();
	
	wait(PULSE_PERIOD);
  uc |= 4; // enable pulse
  i2c_write(uc);
	rc = read_ack();
  
	wait(PULSE_PERIOD);
	uc &= ~4; // toggle pulse
  i2c_write(uc);
	rc = read_ack();
			
	wait(CMD_PERIOD);
	
	i2c_stop();

} /* WriteCommand() */



//
// Write an ASCII string (up to 16 characters at a time)
// 
int lcd1602WriteString(unsigned char iAddr, char *text)
{
unsigned char ucTemp;
int i = 0;
unsigned char rc;

	if (text == NULL)
		return 1;

	i2c_start();
	i2c_write(iAddr);
	rc = read_ack();	
	
	while (i<16 && *text)
	{
		ucTemp = iBackLight | DATA | (*text & 0xf0);
		i2c_write(ucTemp);
		rc = read_ack();
		
		wait(PULSE_PERIOD);
		ucTemp |= 4; // pulse E
		i2c_write(ucTemp);
		rc = read_ack();
		
		wait(PULSE_PERIOD);
		ucTemp &= ~4;
		i2c_write(ucTemp);
		rc = read_ack();
		
		wait(PULSE_PERIOD);
		ucTemp = iBackLight | DATA | (*text << 4);
		i2c_write(ucTemp);
		rc = read_ack();
		
		ucTemp |= 4; // pulse E
    i2c_write(ucTemp);
		rc = read_ack();
    
		wait(PULSE_PERIOD);
    ucTemp &= ~4;
    i2c_write(ucTemp);
		rc = read_ack();
    
		wait(CMD_PERIOD);
		text++;
		i++;
	}
	
	i2c_stop();
	
	return 0;
}


// Open a file handle to the I2C device
// Set the controller into 4-bit mode and clear the display
// returns 0 for success, 1 for failure
//
int lcd1602Init(unsigned char iAddr){
	
	i2c_start();
	i2c_write(iAddr);
	read_ack();
	i2c_stop();
	wait(1000);
	
	iBackLight = BACKLIGHT; // turn on backlight
	WriteCommand(iAddr, 0x02); // Set 4-bit mode of the LCD controller
	WriteCommand(iAddr, 0x28); // 2 lines, 5x8 dot matrix
	WriteCommand(iAddr, 0x0c); // display on, cursor off
	WriteCommand(iAddr, 0x06); // inc cursor to right when writing and don't scroll
	WriteCommand(iAddr, 0x80); // set cursor to row 1, column 1
	lcd1602Clear(iAddr);	    // clear the memory

	return 0;
}

//
// Erase the display memory and reset the cursor to 0,0
//
int lcd1602Clear(unsigned char iAddr)
{
	WriteCommand(iAddr, 0x01); // clear the screen
	return 0;

}

//
// Set the LCD cursor position
//
int lcd1602SetCursor(unsigned char iAddr, unsigned char x, unsigned char y)
{
unsigned char cCmd;

	if (x < 0 || x > 15 || y < 0 || y > 1)
		return 1;

	cCmd = (y==0) ? 0x80 : 0xc0;
	cCmd |= x;
	WriteCommand(iAddr, cCmd);
	return 0;

} /* lcd1602SetCursor() */

//
// Control the backlight, cursor, and blink
// The cursor is an underline and is separate and distinct
// from the blinking block option
//

int lcd1602Control(unsigned char iAddr, unsigned char bBacklight, unsigned char bCursor, unsigned char bBlink)
{
unsigned char ucCMD = 0xc; // display control

	iBackLight = (bBacklight) ? BACKLIGHT : 0;
	if (bCursor)
		ucCMD |= 2;
	if (bBlink)
		ucCMD |= 1;
	WriteCommand(iAddr, ucCMD);
 	
	return 0;
} /* lcd1602Control() */


/*
void lcd1602Shutdown(unsigned char iAddr)
{
	iBackLight = 0; // turn off backlight
	WriteCommand(iAddr, 0x08); // turn off display, cursor and blink	
} /* lcd1602Shutdown() */



