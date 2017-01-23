/*
 * Lab1_ADCandWaveform.c
 *
 *  Created on: Jan 23, 2017
 *      Author: Mike
 */


/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

#define F100HZ 75
#define F20HZ 375
#define F1HZ 7500


//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;
int tempValConversion = 69;
int compareCounter = F100HZ;
int outputState = 0;
double dutyCycle = 20;
volatile int timerCount = 0;

int main(void)
{
	initRBELib();

	//Timer Setup
	TIMSK0 = 0b10; //Enable Interrupt
	TCCR0A=0; //Pin setting and waveform gen setting
	TCCR0B = 0b00000011; //sets clock to (clk/8)
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
	ADMUX =  0b01100111;
	ADCSRA = 0b11100000;
	ADCSRB = 0b00000100;
	ADCSRA |= 0x40; //Starts conversion
	while(1)
	{
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


		if(ADCSRA & 0x10) //if conversion is complete
		{
			/*
			dutyCycle = ADCH;
			dutyCycle = dutyCycle * 0.08 + 10;
			if(dutyCycle < 10)
			{
				dutyCycle = 10;
			}
			if(dutyCycle > 90)
			{
				dutyCycle = 90;
			}
			*/

			dutyCycle +=.001;
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	timerCount++;
	TCNT0=0;
}

void oldCode(void){
	//Enable printf() and setServo()
	initRBELib();

	// Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	//Set the baud rate of the UART
	debugUSARTInit(115200);
	// printf uses a fair amount of memory to be included but makes serial printing much easier
	printf("PutCharDebug is complete \n\r");

	while(1)
	{
		//The get char debug function will return when a character is received
		inchar = getCharDebug();
		//Comment out this line once you have it working correctly
		printf("This line will print when a character is received from the serial connection \n\r");

		if (inchar == 'A')
		{
			//Switch which print statement is commented out when your ready for matlab data collection example
			//matlab will buffer all characters until \n\r
			printf("This will print if the character sent is an A \n\r");
			//printf("%4d\t, %4d,\n\r", lowADC, highADC);
		}
	}
}

