/*
 * Lab1_ADCandWaveform.c
 *
 *  Created on: Jan 23, 2017
 *      Author: Mike
 */

#include "RBELib/RBELib.h"
#include "Lab1_ADCandWaveform.h"
//For use of abs()
#include <stdlib.h>

#define F250HZ 370
#define F100HZ 75
#define F20HZ 375
#define F1HZ 7500


//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;
int tempValConversion = 69;
int compareCounter = F250HZ;
int outputState = 0;
int ADCCount = 0;
double dutyCycle = 50;
volatile int timerCount = 0;

void sample()
{
	int numberOfSamples = 0;
	TCCR0B = 0b00000010;
	int samples[1000];
	//printf("Starting Sampling");

	timerCount = 0;
	while(numberOfSamples < 250)
	{
		if(timerCount > F250HZ)
		{
			if(ADCSRA & 0x10) //if conversion is complete
			{
				int temp = ADCL;
				int tempH = ADCH;
				ADCSRA |= (1<<ADSC); //Starts conversion
				int totalTest = temp + (tempH<<8);
				samples[numberOfSamples] = totalTest;
				numberOfSamples++;
				timerCount=0;
				PORTA = ~PORTA;
			}
		}

	}

	//print samples here
	for(int i = 0; i<250;i++)
	{
		printf("%i,%i\n\r", i,samples[i]);
	}
	TCCR0B = 0b00000011;
	timerCount=0;
}



void Lab1init(void){
	//Timer Setup
	TIMSK0 = 0b10; //Enable Interrupt
	TCCR0A=0; //Pin setting and waveform gen setting
	TCCR0B = 0b00000010; //sets clock to (clk/8)
	OCR0A = 18; //the compare register not supposed to be binary
	sei();


	DDRD = 0xFF;
	PORTD = 0b00001111;
	DDRA = 0xFF;
	PORTA = 0b11111111;

	//Setup Switches

	DDRB = 0; //Port B all to input
	PORTB = 1; //pullup on pin 0


	//Init ADC
	ADMUX = (0<<REFS1)|
			(1<<REFS0)| //AVcc
			(0<<ADLAR)| //Left Adjust for 1024 Value
			(0<<MUX4)|
			(0<<MUX3)|
			(1<<MUX2)| //Sets Analog 7 Pin
			(1<<MUX1)|
			(1<<MUX0);

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

void Lab1loop(void){

	if(~PINB & 0b1) //if PORT B0 is low change frequency
	{
		compareCounter = F1HZ;
	}

	if(~PINB & 0b10) //if PORT B1 is low change frequency
	{
		compareCounter = F20HZ;
	}

	if(~PINB & 0b100) //if PORT B2 is low change frequency
	{
		compareCounter = F100HZ;
	}

	if(~PINB & 0b1000) //if PORT B3 is low start sampling
	{
		sample();
	}

	if(outputState == 0 && timerCount > (compareCounter *2.0 * (100.0 - dutyCycle))/100.0)
	{
		timerCount = 0;
		TCNT0=0;
		PORTA = 0xFF;
		outputState = 1;
	}

	if(outputState == 1 && timerCount > (compareCounter *2.0 * dutyCycle)/100.0)
	{
		timerCount = 0;
		TCNT0=0;
		PORTA = 0;
		outputState = 0;
	}


	if((ADCSRA & 0b00010000) && (ADCCount > F20HZ*4)) //if conversion is complete
	{
		int temp = ADCL;
		int tempH = ADCH;
		int totalTest = temp + (tempH<<8);
		//dutyCycle = totalTest;

		printf("Current ADC Value: %d Low Val: %d High Val: %d\n\r",totalTest,temp,tempH);

		/*
		dutyCycle = dutyCycle * 0.097;
		if(dutyCycle < 10)
		{
			dutyCycle = 10;
		}
		if(dutyCycle > 90)
		{
			dutyCycle = 90;
		}*/


		ADCCount = 0;
		ADCSRA |= (1<<ADSC); //Starts conversion
	}

}

ISR(TIMER0_COMPA_vect){
	timerCount++;
	ADCCount++;
	TCNT0=0;
}
