#include "RBELib/RBELib.h"
#include <stdlib.h>

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
	comAddrByte = (0b0010<<4);//Set command code to write to and update DAC
	comAddrByte |= (DACn & 0b00001111); //Add address code to send data

	unsigned char dataByteOne = SPIVal >> 2;
	unsigned char dataByteTwo = (SPIVal << 6) & 0b11000000;
	spiTransceive(comAddrByte);
	spiTransceive(dataByteOne);//send first 8 data bits
	spiTransceive(dataByteTwo);

	DAC_SS =1;
	//printf("SPIVal is %i\n\r", SPIVal);
	//printf("0x%08X 0x%08X 0x%08X\n\r", comAddrByte, dataByteOne, dataByteTwo);
}

void intiDAC()
{
	int SPIVal=0;
	DAC_SS_ddr = 1; //Set slave select for DAC to output
	DAC_SS = 1; //Set DAC slave select high then pull low for data transmission
	DAC_SS = 0;
	unsigned char comAddrByte;
	comAddrByte = (0b0111<<4);//Set command code to write to and update DAC
	comAddrByte |= (0b00001111); //Add address code to send data

	unsigned char dataByteOne = SPIVal >> 2;
	unsigned char dataByteTwo = (SPIVal << 6) & 0b11000000;
	spiTransceive(comAddrByte);
	spiTransceive(dataByteOne);//send first 8 data bits
	spiTransceive(dataByteTwo);

	DAC_SS =1;
	//printf("SPIVal is %i\n\r", SPIVal);
	//printf("0x%08X 0x%08X 0x%08X\n\r", comAddrByte, dataByteOne, dataByteTwo);
}

double dacToVolts(short adcRead)
{
	if(adcRead<0)
	{
		adcRead *= -1;
	}
	return adcRead*2.39; //map 0->1023 to 0-2500
}
