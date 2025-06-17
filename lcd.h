#ifndef LCD1602_H
#define LCD1602_H

// Init LCD
// Turns on the LCD, sets the 4-bit mode and clears the memory
// Returns 0 for success, 1 for failure
//
int lcd1602Init(unsigned char iAddr);

//
// Set the cursor position on the LCD
//
int lcd1602SetCursor(unsigned char iAddr, unsigned char x, unsigned char y);

//
// Control the backlight, cursor, and blink
//
int lcd1602Control(unsigned char iAddr, unsigned char bBacklight, unsigned char bCursor, unsigned char bBlink);

//
// Print a zero-terminated character string
// Only 1 line at a time can be accessed
//
int lcd1602WriteString(unsigned char iAddr, char *szText);

//
// Clear the characters from the LCD
//
int lcd1602Clear(unsigned char iAddr);

//
// Turn off the LCD and backlight
// close the I2C file handle
//
void lcd1602Shutdown(unsigned char iAddr);



#endif // LCD1602_H