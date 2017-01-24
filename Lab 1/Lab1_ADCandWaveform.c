/*
 * Lab1_ADCandWaveform.c
 *
 *  Created on: Jan 23, 2017
 *      Author: Mike
 */


/* main.c
 *
 *  Created on: January 22nd, 2017
 *      Author: mphopkins
 */


#include "Lab1_ADCandWaveform.h"


//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;
int tempValConversion = 69;
int compareCounter = F100HZ;
int outputState = 0;
double dutyCycle = 20;
volatile int timerCount = 0;

void Lab1init(void){
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

ISR(TIMER0_COMPA_vect){
	timerCount++;
	TCNT0=0;
}
