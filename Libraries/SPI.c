/*
 * SPI.c
 *
 *  Created on: Jan 31, 2017
 *      Author: Mike
 */
#include "RBELib/RBELib.h"

void initSPI()
{
	SPI_MISO_DDR = 0;// MISO set to input
	SPI_MISO =0; //Set MISO to low

	SPI_MOSI_DDR=1;
	SPI_MOSI =0; //MOSI set to output and low

	SPI_SCK_DDR=1;
	SPI_SCK=0; //SCK set to output and low

	DDRB |= (1<<DDB4);

	SPCR = 	 (0<<SPIE)| //Do not enable interrupt
			 (1<<SPE)|  //Enable SPI
			 (0<<DORD)|	//MSB Transmitted first
			 (1<<MSTR)|	//Select Master Mode
			 (0<<CPOL)| //Positive clock signal
			 (0<<CPHA)| //Send bit on rising edge of clock
			 (1<<SPR1)|	//Set clock to Fosc/64
			 (0<<SPR0);	//Set clock to Fosc/64

	SPSR =	(0<<SPI2X); //Do not double speed
}

unsigned char spiTransceive(BYTE data)
{
	SPDR = data;
	while(!((SPSR) & (1<<SPIF))); //Wait until data is done being sent
	//printf("Data that was sent: %i\n\r",data);
	//recievedData = SPDR;
	//printf("",recievedData);
	return SPDR; //Return the data transmitted back by SPI device
}
