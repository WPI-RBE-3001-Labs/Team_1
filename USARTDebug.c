/*
 * USART.c
 *
 *  Created on: Jan 26, 2015
 *      Author: joest
 */


#include "RBELib/USARTDebug.h"
#include "RBELib/RBELib.h"
#include <avr/io.h>
#include <stdlib.h>

unsigned long freqOSC = (1840000);

void debugUSARTInit(unsigned long baudrate){
	unsigned long myubrr = 9;//((freqOSC)/(baudrate*16UL)-1);

	UBRR1H = (myubrr>>8); //shifts the register to the right by 8 bits which splits up UBRR
	UBRR1L = myubrr;
	UCSR1B |= 0b00011000;  //Enables TXEN and RXEN
	UCSR1C |= 0b00000110;  //Enables Async USART, no Parity, 1 Stop bit, 8 bit character size
}

void putCharDebug(char byteToSend){
	while (!(UCSR1A & 0b00100000)); // wait while the register is free
	UDR1 = byteToSend; //upload data to register
}

unsigned char getCharDebug(void) {
	while(!(UCSR1A & 0b10000000));
	return UDR1;
}
