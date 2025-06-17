#include "I2C_bitbang.h"

/*....................................................................
 * This function generates a small wait cycle
 *....................................................................*/
void i2c_wait(void){
	
	unsigned int i; 
	
	for(i = 0; i < I2CWAIT; ){
		i++;
	}
}

//....................................................................
// This function generates an I2C Start Condition
//....................................................................
void i2c_start(void){	
	SDA = 1;						// ensure SDA & SCL are high
	SCLK = 1;
	
	i2c_wait();					// wait some cycles
	i2c_wait();					// wait some cycles
	
	SDA = 0;            // pull SDA low
	i2c_wait();
	i2c_wait();
	SCLK = 0;           // pull SCL low
	i2c_wait();
	i2c_wait();
}

//....................................................................
// This function generates an I2C Stop Condition
//....................................................................
void i2c_stop(void){
	SCLK = 0;						// ensure SCL is low
	SDA = 0;            // SDA low
	
	i2c_wait();					// wait some cycles

	SCLK = 1;           // pull SCL high
	
	i2c_wait();					// wait some cycles

	SDA = 1;            // allow SDA to be pulled high
	
	i2c_wait();					// wait some cycles
}

//....................................................................
// Writes a byte to the I2C bus
//....................................................................
unsigned char i2c_write(unsigned char wdata){
	unsigned char i;						// loop counter

	SCLK = 0;										// ensure SCL is low
	
	for (i = 0; i < 8; i++)			// loop through each bit
	{
		SDA = (wdata >> 7);				// output the MSB

		i2c_wait();								// wait some cycles
		i2c_wait();
		
		SCLK = 1;                	// pull SCL high to clock bit
		i2c_wait();								// wait some cycles
		i2c_wait();
		SCLK = 0;							// wait some cycles
		
		wdata = wdata << 1;				// shift left for next bit
	}
	
#ifdef I2C_DEBUG
	// for debug purposes, SDA goes low for the ACK receive
	//SDA = 0;
#endif
	
	return 0;
}

//....................................................................
// Reads a byte from the I2C bus
//....................................................................
unsigned char i2c_read(void)
{
	unsigned char i;                // loop counter
	unsigned char rdata = 0;     		// input value
	
	for (i = 0; i < 8; i++)         // loop through each bit
	{
		rdata = rdata << 1;             // shift left for next bit

		SCLK = 0;                        // ensure SCL is low
		SCLK = 1;                        // bring SCL high to begin transfer
		
		i2c_wait();

		rdata |= SDA;                   // input the received bit
			
		SCLK = 0;                        // bring SCL low again.
		
		i2c_wait();
			
  }
				
	return rdata;
}

//....................................................................
// Read ack bit from the I2C bus
//....................................................................
unsigned char read_ack(void){
	
	unsigned char ack;
		
	SCLK = 0;                       // ensure SCL is low

	SCLK = 1;                       // bring SCL high to begin transfer
	
	i2c_wait();											// wait some cycles
	i2c_wait();											// wait some cycles
	i2c_wait();											// wait some cycles
	
	ack = SDA;                   	// input the received bit.
	
	SCLK = 0;                      	// bring SCL low again.
	
	i2c_wait();											// wait some cycles
	
	return ack;
}

//....................................................................
// Write ack bit to the I2C bus
//....................................................................
void write_ack(unsigned char ack)
{
	SCLK = 0;                        // ensure SCL is low

	SCLK = 1;                        // bring SCL high to begin transfer
	i2c_wait();
	i2c_wait();
	
	SDA = (ack >> 7);                 // output the MSB
	SCLK = 1;                        // pull SCL high to clock bit	
	i2c_wait();
	
	SCLK = 0;                        // bring SCL low again.
	
}
