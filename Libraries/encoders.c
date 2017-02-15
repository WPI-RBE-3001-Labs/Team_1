/*
 * encoders.c
 *
 *  Created on: Feb 13, 2017
 *      Author: djcupo
 */
#include "encoders.h"
#include "RBELib/RBELib.h"
#include <stdlib.h>



int encoderCounts(int chan)
{
	int output = 0;
	signed long temp1 = 0;
	signed long temp0 = 0;
	switch(chan){
	case 0:
		ENCODER_SS_0_ddr = 1;
		ENCODER_SS_0 = 1;

		ENCODER_SS_0 = 0;

		spiTransceive(READ_CNTR);
		temp1 = spiTransceive(0);
		temp0 = spiTransceive(0);

		output = (temp1<<8)+(temp0);

		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;

		spiTransceive(CLR_CNTR);

		ENCODER_SS_0 = 1;

		return output;
		break;
	case 1:
		ENCODER_SS_1_ddr = 1;
		ENCODER_SS_1 = 1;

		ENCODER_SS_1 = 0;

		spiTransceive(READ_CNTR);
		temp1 = spiTransceive(0);
		temp0 = spiTransceive(0);

		output = (temp1<<8)+(temp0);

		ENCODER_SS_1 = 1;
		ENCODER_SS_1 = 0;

		spiTransceive(CLR_CNTR);

		ENCODER_SS_1 = 1;
		return output;
		break;
	default:
		return -1;
	}
}
void initEncoder(int chan)
{
	switch(chan){
	case 0:
		ENCODER_SS_0_ddr = 1;

		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;

		spiTransceive(WRITE_MDR0);
		spiTransceive(QUADRX4|FREE_RUN|FILTER_1|DISABLE_INDX);


		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;

		spiTransceive(WRITE_MDR1);
		spiTransceive(0x02);

		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;

		spiTransceive(CLR_CNTR);
		ENCODER_SS_0 = 1;


		break;
	case 1:
		ENCODER_SS_1_ddr = 1;
		ENCODER_SS_1 = 1;

		ENCODER_SS_1 = 0;

		spiTransceive(WRITE_MDR0);
		spiTransceive(QUADRX4|FREE_RUN|FILTER_1|DISABLE_INDX);
		ENCODER_SS_1 = 0;

		spiTransceive(CLR_CNTR);
		ENCODER_SS_1 = 1;
		ENCODER_SS_1 = 0;

		spiTransceive(WRITE_MDR1);
		spiTransceive(0x02);

		ENCODER_SS_1 = 1;
		break;
	}


}

void resetEncoder(int chan){
	switch(chan){
	case 0:
		ENCODER_SS_0_ddr = 1;
		ENCODER_SS_0 = 1;
		ENCODER_SS_0 = 0;

		spiTransceive(CLR_CNTR);

		ENCODER_SS_0 = 1;
		break;
	case 1:
		ENCODER_SS_1_ddr = 1;
		ENCODER_SS_1 = 1;
		ENCODER_SS_1 = 0;

		spiTransceive(CLR_CNTR);

		ENCODER_SS_1 = 1;
		break;
	}
}
