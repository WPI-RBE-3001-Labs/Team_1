/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
#include "RBELib/USARTdebug.h"
#include "Lab1/Lab1_ADCandWaveform.h"
#include "Lab2A.h"
#include "Lab3.h"
//For use of abs()
#include <stdlib.h>

int main(void)
{

	initRBELib();
	debugUSARTInit(115200);
	Lab3Init();
	printf("\n\rMain.c\n\r");


	while(1){
		Lab3Loop();
	}
	return 0;
}

