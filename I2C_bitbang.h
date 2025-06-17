#ifndef _I2C_BITBANG_
#define _I2C_BITBANG_

#include <REG51F380.H>

sbit SDA	= P0^2;
sbit SCLK	= P0^3;

#define xI2C_DEBUG
#define ACK       0
#define NACK      0x01
#define I2CWAIT		20

void i2c_start(void);															// generates an I2C start condition
void i2c_stop(void);															// generates an I2C stop condition
unsigned char i2c_write(unsigned char i2c_data);  // writes a byte to the I2C bus
unsigned char i2c_read(void);											// reads a byte from the I2C bus
unsigned char read_ack(void);											// reads the ACK/NACK signal
void write_ack(unsigned char ack);								// write ACK signal

#endif