

/*
* main.c
*
* Created on: Sep 14, 2016
* Author: Kenneth
*/
#include <RBELib/RBELib.h> //RBELib
#include <avr/io.h>
#include <stdlib.h>

unsigned int aDelay;
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) // calculation to set baudrate in the registers



//this function initializes UART

void UART_init (void)
{

	UBRR1H = (BAUDRATE>>8); //shits the register to the right by 8 bits which splits up UBRR
	UBRR1L = BAUDRATE;
	UCSR1B |= 0b00011000; //(1<<TXEN)|(1<<RXEN); //sets the receiving and transmitting bits to one so they are enabled
	UCSR1C |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1); //8 bit data format

}



void uart_transmit (unsigned char data)
{

	while (1(UCSR1A & (1<<UDRE))); // wait while the register is free
	UDR = data; //upload data to register

}



unsigned char uart_receive (void)
{

	while(!(UCSR1A) & (1<<RXC));
	return UDR;

}
