/*
 * Lab2A.c
 *
 *  Created on: Jan 27, 2017
 *      Author: Mike Hopkins
 */
#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

void Lab2AInit()
{
	//initADC(7);
}

void Lab2ALoop()
{
	//printf("Current ADC Value: %i\n\r",getADC(7));
	if((ADCSRA & 0b00010000)) //if conversion is complete
		{
			int temp = ADCL;
			int tempH = ADCH;
			int totalTest = temp + (tempH<<8);

			printf("Current ADC Value:- %d\n\r",totalTest);
			ADCSRA |= (1<<ADSC); //Starts conversion
		}
}

void initADC(int channel)
{

	ADMUX = (0<<REFS1)|
			(1<<REFS0)| //AVcc
			(0<<ADLAR)| //Left Adjust for 1024 Value
			(0<<MUX4)|
			(0<<MUX3)|
			(1<<MUX2)| //Sets Analog 7 Pin
			(1<<MUX1)|
			(1<<MUX0);

			//(channel); // Sets MUX2:0 to proper input channel

	ADCSRA = (1<<ADEN)|
			(0<<ADSC)|
			(0<<ADATE)|
			(0<<ADIF)|
			(0<<ADIE)|
			(1<<ADPS2)|
			(1<<ADPS1)| //sets division factor to 64
			(0<<ADPS0);
	ADCSRB =(0<<ACME)|
			(0<<ADTS2)|
			(0<<ADTS1)| //Sets ADC to free running mode
			(0<<ADTS0);
}

unsigned short getADC(int channel)
{
	unsigned short lowByte=0;
	unsigned short highByte=0;
	unsigned short output=0;

	ADMUX = (0<<REFS1)|
			(1<<REFS0)| //AVcc
			(0<<ADLAR)| //Left Adjust for 1024 Value
			(0<<MUX4)|
			(0<<MUX3)|
			(1<<MUX2)| //Sets Analog 7 Pin
			(1<<MUX1)|
			(1<<MUX0);
			//(channel); // Sets MUX2:0 to proper input channel
	ADCSRA |= (1<<ADSC); //Starts conversion
	while(ADCSRA &= (1<<ADIF));//wait for conversion to complete

	lowByte = ADCL;
	highByte= ADCH;
	output = lowByte + (highByte<<8);
	printf("Output Value: %i",lowByte);

	return output;
}

