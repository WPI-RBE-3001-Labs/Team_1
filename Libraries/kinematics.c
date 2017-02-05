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

double getX(double theta1, double theta2){
	theta1 = theta1 * 3.14 / 180;
	theta2 = theta2 * 3.14 / 180;
	double x1 = LENGTH1 * cos(theta1);
	double x2 = x1 + LENGTH2 * cos(theta1 + theta2);
	return x2;
}
double getY(double theta1, double theta2){
	theta1 = theta1 * 3.14 / 180;
	theta2 = theta2 * 3.14 / 180;
	double y1 = LENGTH1 * sin(theta1);
	double y2 = y1 + LENGTH2 * sin(theta1 + theta2);
	return y2;
}

