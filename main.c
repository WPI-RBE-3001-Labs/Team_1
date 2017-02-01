/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
#include "RBELib/USARTdebug.h"
#include "Lab1/Lab1_ADCandWaveform.h"
#include "Lab2A.h"
//For use of abs()
#include <stdlib.h>

int main(void)
{

	initRBELib();
	debugUSARTInit(115200);
	Lab2AInit();
	//printf("\n\rMain.c\n\r");


	while(1){
		Lab2ALoop();
	}
	return 0;
}

