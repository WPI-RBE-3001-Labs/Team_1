/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
#include "RBELib/USARTdebug.h"
#include "Lab1/Lab1_ADCandWaveform.h"
#include "Lab2A.c"
//For use of abs()
#include <stdlib.h>

int main(void) //For Lab2A
{
	initRBELib();
	debugUSARTInit(115200);
	Lab2AInit();
	printf("\n\rMain.c");


	while(1){
		_delay_ms(100);
		Lab2ALoop();
	}
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

