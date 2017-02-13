/*
 * accel.c
 *
 *  Created on: Feb 13, 2017
 *      Author: Mike
 */
#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

//return unit of 0.01g
signed int getAccel(int axis)
{
	int input = 0;
	int vRef = 0;
	int MSBInput = 0;
	int LSBInput = 0;
	BYTE control =  0b11000; //set single mode instead of differential
	control |= axis;
	BYTE readVref = 0b11011; //command single mode channel 3 to read Vref
	ACC_SS = 0;

	MSBInput = spiTransceive(control);//send the channel we want to read and read top 4 bits
	LSBInput = spiTransceive(0);//send nothing. Recieve next 8 bits

	input = (MSBInput<<8)|LSBInput;

	ACC_SS = 1;
	ACC_SS = 0;

	MSBInput = spiTransceive(readVref);//send the channel we want to read and read top 8 bits
	LSBInput = spiTransceive(0);//send nothing. Recieve next 8 bits

	vRef = (MSBInput<<8)|LSBInput;
	ACC_SS = 1;

	return (input-vRef)*0.22;
}
