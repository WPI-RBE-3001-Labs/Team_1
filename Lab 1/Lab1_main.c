/*
 * Lab1_main.c
 *
 *  Created on: Jan 24, 2017
 *      Author: djcupo
 */
#include "Lab1_main.h"

int main(void){

	initRBELib();
	debugUSARTInit(115200);

	while(1){
		printf("It Works!\n");
		_delay_ms(500);
	}
}
