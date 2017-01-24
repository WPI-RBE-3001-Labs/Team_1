/*
 * USARTDebug.c
 *
 *  Created on: Jan 22, 2017
 *      Author: Domin
 */

#include "RBELib/USARTDebug.h"
#include "RBELib/RBELib.h"
#include <avr/io.h>
#include <stdlib.h>
#include "GlobalDefines.h"

unsigned int aDelay;

const static unsigned int FOSC = 1843200;

/**
 * @brief Initializes USART1 as a print terminal to the PC. This function
 * must check the incoming baudrate against the valid baudrates
 * from the data-sheet. If the baudrate is invalid, then the
 * DEFAULT_BAUD constant must be used instead.
 *
 * @param baudrate The desired baudrate to set for USART1.
 *
 * @todo Create the function that will initialize USART1 for debugging use.
 */
void debugUSARTInit(unsigned long baudrate){

	unsigned long myubrr = ((FOSC)/(baudrate*16UL)-1)
	UBRR1H = (myubrr>>8); //shifts the register to the right by 8 bits which splits up UBRR
	UBRR1L = myubrr;
	UCSR1B = 0b00011000; //(1<<TXEN1)|(1<<RXEN1); //sets the receiving and transmitting bits to one so they are enabled
	UCSR1C = 0b01000110; //(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1) 8 bit data format
}

/**
 * @brief Sends one byte to the USART1 Tx pin (Transmits one byte).
 *
 * @param byteToSend The byte that is to be transmitted through USART1.
 *
 * @todo Make the function that will put a character on the USART1 TX line.
 */
void putCharDebug(char byteToSend){
	while (!(UCSR1A & BIT5)); // wait while the register is free
	UDR1 = byteToSend; //upload data to register
}

/**
 * @brief Recieves one byte of data from the serial port (i.e. from the PC).
 *
 * @return byteReceived Character that was received on the USART.
 *
 * @todo Make the function that will listen for input on the USART1 RX line.
 */
unsigned char getCharDebug(void){
	while(!(UCSR1A) & BIT7);
	return UDR1;
}
