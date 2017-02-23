/*
 * rangeSensor.c
 *
 *  Created on: Feb 23, 2017
 *      Author: Mike
 */
#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include "ADC.h"
#include "math.h"

float getIRInches(int channel)
{
	float input = getADC(channel);
	input = 1501*pow(input,-0.815);
	return (input-4.0663)/1.3847;
}

float getIRmm(int channel)
{
	return getIRInches(channel) * 25.4;
}
