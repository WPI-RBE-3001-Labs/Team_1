/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
#include "RBELib/USARTdebug.h"
#include "Lab1/Lab1_ADCandWaveform.h"
#include "Lab2A/Lab2A.h"
//For use of abs()
#include <stdlib.h>

int main(void) //For Lab2A
{
	initRBELib();
	debugUSARTInit(115200);
	printf("\n\rMain.c\n\r");
	Lab2AInit();
	_delay_ms(500);



	while(1){
		Lab2ALoop();
	}
	return 0;
}

int mainLab1(void)
{

	initRBELib();
	debugUSARTInit(115200);
	Lab1init();
	printf("\n\rMain.c");


	while(1){
		Lab1loop();
	}
	return 0;
}

