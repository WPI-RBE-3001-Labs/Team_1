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

//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;
int tempValConversion = 69;
volatile int timerCount = 0;

int runADCandWaveformGen(void)
{
	initRBELib();

	//Timer Setup
	TIMSK0 = 0b10; //Enable Interupt
	TCCR0A=0; //Pin setting and waverform gen setting
	TCCR0B = 0b00000101; //sets clock to 18000Hz  (clk/1024)
	OCR0A = 18; //the compare register not supposed to be binary
	sei();


	DDRD = 0xFF;
	PORTD = 0b00001111;
	DDRA = 0xFF;
	PORTA = 0b11111111;
	 //Init ADC
	ADMUX =  0b01100111;
	ADCSRA = 0b11100000;
	ADCSRB = 0;
	printf("I hope this works!");



	while(1){




		ADCSRA |= 0x40; //Starts conversion
		//PORTA=0;

		if(timerCount > 40)
		{
			timerCount = 0;
			TCNT0=0;
			PORTA = ~PORTA;
		}

		/*
		if(ADCSRA & 0x10) //if conversion is complete
		{
			PORTA = 0;
			tempValConversion = ADCH;
			PORTD = tempValConversion;
			printf("%i", tempValConversion);
		}*/
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

