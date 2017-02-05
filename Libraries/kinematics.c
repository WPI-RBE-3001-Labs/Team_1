/*
 * kinematics.c
 *
 *  Created on: Feb 1, 2017
 *      Author: Mike
 */

#include "kinematics.h"
#include <math.h>

//Convert To Degrees**********************

int signTheta2(double x,double y)
{
	double theata2 = xyToTheta2(x,y);
	double theata1 = xyToTheta1(x,y,theata2);
	if(theata1 < -3)
	{
		theata2 *= -1;
		theata1 = xyToTheta1(x,y,theata2);
		if(theata1<-3)
		{
			return 0;
		}else{
			return -1;
		}
	}
	return 1;
}

double xyToTheta1(double x, double y,double theata2)
{
	//double theata2 = xyToTheta2(x,y);
	return (atan2(LENGTH2*sin(theata2)*x+LENGTH1+(LENGTH2*cos(theata2)*y),(LENGTH1+LENGTH2*cos(theata2))*x-LENGTH2*sin(theata2)*y))*(180/3.1415);
}

double xyToTheta2(double x, double y)
{
	double output = acos((x*x+y*y-LENGTH1*LENGTH1-LENGTH2*LENGTH2)/(2*LENGTH1*LENGTH2))*(180/3.1415);
	return output;
}
