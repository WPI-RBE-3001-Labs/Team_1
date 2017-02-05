/*
 * SPI.c
 *
 *  Created on: Jan 31, 2017
 *      Author: Mike
 */

#include "RBELib/RBELib.h"
#include "ADC.h"

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

int adcToDegreesArm1(short adcRead)
{
	return (int) (adcRead * 90.0 /(ADCVERTARM1-ADCHORZARM1))-45;
}

double degreesToADCArm1(short degrees)
{
	return ((degrees + 45)*(ADCVERTARM1-ADCHORZARM1))/90;
}

double adcToDegreesArm2(short degrees)
{
	return (180.0/(ADCHORZPOSARM2-ADCHORZNEGARM2))*degrees - 143;
}
/**
 * @brief Convert ADC value to millivolts
 *
 * @param adcRead The ADC reading given as a range between 0-1023
 *
 */
int adcToVolts(int adcRead)
{
	if(adcRead<0)
	{
		adcRead *= -1;
	}
	return adcRead*239/50; //map 0->1023 to 0-5000
}
int ADCtoMillamps(int adcRead){
	float ratio = 4.3096;
	return (int) (adcRead-536.53)*ratio;
}
