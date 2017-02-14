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
	BYTE control =  0b110; //set single mode instead of differential
	ACC_DDR = 1; //set slave select to output than toggle high to low to start first command
	ACC_SS = 1;
	ACC_SS = 0;

	//As per datasheet send start bit then set single mode in last 4 bits of byte.
	//Then send the axis on bit 6-7
	//after receiving the axis the ADC will output the 12bit result. We can only read
	//8 at a time so we receive twice then bit shift and mask
	spiTransceive(control);
	MSBInput = spiTransceive(axis<<6);//send the channel we want to read and read top 4 bits
	LSBInput = spiTransceive(0);//send nothing. Receive next 8 bits

	input = ((MSBInput<<8)|LSBInput) & 0x0FFF;
	//printf("MSB: %i, LSB: %i, Input: %i\n\r",MSBInput,LSBInput,input);

	ACC_SS = 1; //Toggle slave select to start next command
	ACC_SS = 0;

	//As per datasheet send start bit then set single mode in last 4 bits of byte.
	//Then send the axis on bit 6-7
	//after receiving the axis the ADC will output the 12bit result. We can only read
	//8 at a time so we receive twice then bit shift and mask
	spiTransceive(control);
	MSBInput = spiTransceive(0b11<<6);//send the channel we want to read and read top 8 bits
	LSBInput = spiTransceive(0);//send nothing. Recieve next 8 bits

	vRef = ((MSBInput<<8)|LSBInput) & 0x0FFF;
	ACC_SS = 1;
	//printf("MSB: %i, LSB: %i, VRef: %i\n\r",MSBInput,LSBInput,vRef);
	//printf("Return: %i\n\r", (signed int)((input-vRef)*0.22));
	return (signed int) ((input-vRef)*0.22);
}
