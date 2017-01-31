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

//int mainLab2(void) //For Lab2A
//{
//	initRBELib();
//	debugUSARTInit(115200);
//	printf("\n\rMain.c\n\r");
//	Lab2AInit();
//	_delay_ms(500);
//
//
//
//	while(1){
//		Lab2ALoop();
//	}
//	return 0;
//}

int main(void)
{

	initRBELib();
	debugUSARTInit(115200);
	Lab2AInit();
	printf("\n\rMain.c\n\r");


	while(1){
		Lab2ALoop();
	}
	return 0;
}

