/*
 * motors.c
 *
 *  Created on: Jan 31, 2017
 *      Author: Mike
 */

#include "RBELib/RBELib.h"
#include "motors.h"

/**
 *@brief Drives a motor based on a signed control signal between -1023 and 1023
 *
 *@param speed a value between -1023 and 1023 represented how fast the motor shoud be moved
 *@param motor a value of either 0 (SHOULDER_MOTOR) or 1 (ELBOW_MOTOR)
 */
void driveMotor(int motor,int speed)
{
	//printf("DriveMotor\n\r");
	if(speed>1023)
	{
		speed=1023;
	}
	if(speed<-1023)
	{
		speed=-1023;
	}
	if(speed > -30 && speed < 30){
		speed=0;
	}
	if((speed>=0) && (motor == SHOULDER_MOTOR))
	{
		setDAC(0,speed);
		setDAC(1,0);
		//printf("Forward\n\r");
	}

	if((speed<0) && (motor == SHOULDER_MOTOR))
	{
		setDAC(0,0);
		setDAC(1,speed*-1);
		//printf("Backward");
	}
	/*
	if((speed>=0) & (motor == ELBOW_MOTOR))
	{
		setDAC(2,speed);
		setDAC(3,0);
	}

	if((speed<0) & (motor == ELBOW_MOTOR))
	{
		setDAC(2,0);
		setDAC(3,speed*-1);
	}*/
}
