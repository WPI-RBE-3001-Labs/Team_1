/*
 * Lab2A.c
 *
 *  Created on: Jan 27, 2017
 *      Author: Mike Hopkins
 */


#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include "Lab2A.h"

int DACVal = 0;
int direction =1;

void Lab2AInit()
{
	initADC(2);
	initSPI();
	setDAC(0,600);
	//printf("ADC Value,Output(voltage),Position(degrees)\n\r");
}

void Lab2ALoop()
{

	setDAC(0,DACVal);
	//printf("DACVal: %i  %i\n\n\r",DACVal,direction);
	_delay_ms(1000);
	if(direction==1)
	{
		DACVal += 100;

		if(DACVal>1024)
		{
			direction =0;
		}
	}else{
		DACVal-=100;
		if(DACVal<0)
		{
			direction=1;
		}
	}



}

void initADC(int channel)
{

	ADMUX = (0<<REFS1)|
				(1<<REFS0)| //AVcc
				(0<<ADLAR)| //Left Adjust for 1024 Value
				(0<<MUX4)|
				(0<<MUX3)|
				(channel); //Sets MUX2:0 to proper input channel

		ADCSRA = (1<<ADEN)|
				(0<<ADSC)|
				(0<<ADATE)|
				(0<<ADIF)|
				(0<<ADIE)|
				(1<<ADPS2)|
				(1<<ADPS1)| //sets division factor to 64
				(0<<ADPS0);
		ADCSRB = 0b00000000;
		DIDR0 =  0b10000000;
		ADCSRA |= (1<<ADSC); //Starts conversion
}

unsigned short getADC(int channel)
{
	unsigned short lowByte=0;
	unsigned short highByte=0;
	unsigned short output=-55;


	ADMUX = (0<<REFS1)|
			(1<<REFS0)| //AVcc
			(0<<ADLAR)| //Left Adjust for 1024 Value
			(0<<MUX4)|
			(0<<MUX3)|
			(channel); //Sets MUX2:0 to proper input channel
	ADCSRA |= (1<<ADSC);

	while(1)//wait for conversion to complete
	{
		if((ADCSRA & 0b00010000))
		{
		lowByte = ADCL;
		highByte= ADCH;
		output = lowByte + (highByte<<8);
		//printf("Output Value: %i",output);

		return output;
		}
	}
}

double adcToDegreesArm1(short adcRead)
{
	return (adcRead * 90.0 /(ADCVERTARM1-ADCHORZARM1))-45;
}

/**
 * @brief Convert ADC value to millivolts
 *
 * @param adcRead The ADC reading given as a range between 0-1023
 *
 */
double adcToVolts(short adcRead)
{
	return adcRead*4.78; //map 0->1023 to 0-5000
}

void setDAC(int DACn, int SPIVal)
{
	if(SPIVal>1023) //Ensure that SPIVal is within constraints
	{
		SPIVal=1023;
	}
	if(SPIVal<0)
	{
		SPIVal=0;
	}

	DAC_SS_ddr = 1; //Set slave select for DAC to output
	DAC_SS = 1; //Set DAC slave select high then pull low for data transmission
	DAC_SS = 0;
	unsigned char comAddrByte;
	comAddrByte = (0b0011<<4);//Set command code to write to and update DAC
	comAddrByte |= (DACn & 0b00001111); //Add address code to send data

	unsigned char dataByteOne = SPIVal >> 2;
	unsigned char dataByteTwo = (SPIVal << 6) & 0b11000000;
	spiTransceive(comAddrByte);
	spiTransceive(dataByteOne);//send first 8 data bits
	spiTransceive(dataByteTwo);
	//printf("SPIVal is %i\n\r", SPIVal);
	//printf("0x%08X 0x%08X 0x%08X\n\r", comAddrByte, dataByteOne, dataByteTwo);
}

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
	int recievedData=-1;
	SPDR = data;
	while(!((SPSR) & (1<<SPIF))); //Wait until data is done being sent
	//printf("Data that was sent: %i\n\r",data);
	recievedData = SPDR;
	//printf("",recievedData);
	return recievedData; //Return the data transmitted back by SPI device
}
