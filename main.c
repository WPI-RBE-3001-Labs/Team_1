/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
#include "RBELib/USARTdebug.h"
#include "Lab1_ADCandWaveform.h"
//For use of abs()
#include <stdlib.h>

int main(void)
{

	initRBELib();
	debugUSARTInit(115200);
	Lab1init();
	printf("\n\rMain.c");


	while(1){
		Lab1loop();
	}
}

