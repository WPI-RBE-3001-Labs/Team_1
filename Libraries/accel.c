/*
 * accel.c
 *
 *  Created on: Feb 13, 2017
 *      Author: Mike
 */
#include "accel.h"

//return unit of 0.01g
signed int GetAccelerationH48C(int axis)
{
	int input = 0;
	int vRef = 0;
	int MSBInput = 0;
	int LSBInput = 0;
	BYTE control =  0b11000; //set single mode instead of differential
	control |= axis;
	control = control<<3;
	BYTE readVref = 0b11011000; //command single mode channel 3 to read Vref
	ACC_DDR = 1;
	ACC_SS = 1;
	ACC_SS = 0;


	MSBInput = spiTransceive(control);//send the channel we want to read and read top 4 bits
	LSBInput = spiTransceive(0);//send nothing. Recieve next 8 bits

	input = ((MSBInput<<8)|LSBInput) & 0x0FFF;
	//printf("MSB: %i, LSB: %i, Input: %i\n\r",MSBInput,LSBInput,input);

	ACC_SS = 1;
	ACC_SS = 0;

	MSBInput = spiTransceive(readVref);//send the channel we want to read and read top 8 bits

	LSBInput = spiTransceive(0);//send nothing. Recieve next 8 bits

	vRef = ((MSBInput<<8)|LSBInput) & 0x0FFF;
	ACC_SS = 1;
	//printf("MSB: %i, LSB: %i, VRef: %i\n\r",MSBInput,LSBInput,vRef);
	//printf("Return: %i\n\r", (signed int)((input-vRef)*0.22));
	return (signed int) ((input-vRef)*0.22);
}
