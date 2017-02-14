/*
 * Lab3.c
 *
 *  Created on: Feb 13, 2017
 *      Author: Mike
 */


#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include "Libraries/accel.h"
#include "Libraries/encoders.h"
void Lab3Init()
{
	initSPI();
	initEncoder(0);

	//printf("Desired Position: (%i,%i)  Theta1: %f  Theta2: %f\n\r",x,y,xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y)),signTheta2(x,y)*xyToTheta2(x,y));
//	desiredValueShoulder = xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y));
//	desiredValueElbow = signTheta2(x,y)*xyToTheta2(x,y);

	//intiDAC();

	printf("Lab3.c");
}

signed int xG=0;
signed int yG=0;
signed int zG=0;
long total =0;
long temp=0;
void Lab3Loop()
{

	xG = GetAccelerationH48C(0);
	yG = GetAccelerationH48C(1);
	zG = GetAccelerationH48C(2);

	printf("X:%i, Y: %i, Z: %i\n\r",xG,yG,zG);
	_delay_ms(500);
	/*
	temp = encoderCounts(0);
	total+=temp;
	printf("Ticks: %lu, Total:%lu\n\r",temp,total);
	_delay_ms(1000);*/
}
