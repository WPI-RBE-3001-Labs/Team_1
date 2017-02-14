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
#include "Lab3.h"
#include "Libraries/motors.h"
#include "Libraries/ADC.h"
#include "Libraries/DAC.h"

//ISR Variables
volatile int HzFlag = 0;

//PID Vaiables
int integrationSumShoulder = 0;
double integrationSumElbow = 0;

double KpShoulder = 45;
double KiShoulder = 0.03;
double KdShoulder = 0;

double KpElbow = 20;
double KiElbow = 0.01;
double KdElbow = 0;

int lastPosShoulder = 0;
int lastPosElbow = 0;
int lastPIDOutputShoulder = 0;
int lastPIDOutputElbow = 0;
volatile unsigned int timer = 0;
double desiredValueShoulder=ADCHORZARM1;
double desiredValueElbow=0;
int currentVal;
int oldValShoulder = -999;
int oldValElbow = -999;
int error;
int diffErr;
int counter=0;


void Lab3Init()
{
	initSPI();
	initEncoder(0);
	TimerInit100Hz();

	//Configure Buttons by setting pins to input
	DDRD &= ~((1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3));

	//printf("Desired Position: (%i,%i)  Theta1: %f  Theta2: %f\n\r",x,y,xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y)),signTheta2(x,y)*xyToTheta2(x,y));
//	desiredValueShoulder = xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y));
//	desiredValueElbow = signTheta2(x,y)*xyToTheta2(x,y);

	//intiDAC();

	printf("Lab3.c");
}

signed int xG=0;
signed int yG=0;
signed int zG=0;
signed long total =0;
signed long temp=0;
int setSpeed=0;
void Lab3Loop()
{

	xG = GetAccelerationH48C(0);
	yG = GetAccelerationH48C(1);
	zG = GetAccelerationH48C(2);

	printf("X:%i, Y: %i, Z: %i\n\r",xG,yG,zG);
	_delay_ms(100);
//	if(HzFlag == 1)
//	{
//		temp = encoderCounts(0);
//		updatePID(desiredValueShoulder,SHOULDER_MOTOR);
//	}
//
//	if(counter>30)
//	{
//		counter=0;
//		driveMotor(SHOULDER_MOTOR,lastPIDOutputShoulder);
//		printf("Desired Value Shoulder: %f, Current Value: %i,Shoulder PID: %i\n\r", desiredValueShoulder,currentVal,lastPIDOutputShoulder);
//
//	}
//	if(((~PIND) & (1<<PIND0))!=0)
//	{
//		desiredValueShoulder = ADCHORZARM1;
//		_delay_ms(20);
//	}
//
//	if(((~PIND) & (1<<PIND1))!=0)
//	{
//		desiredValueShoulder = ADCVERTARM1;
//		_delay_ms(20);
//	}

//	driveMotor(SHOULDER_MOTOR,setSpeed);
//
//	//THIS IS FOR THE DATA COLLECTION WITH MOTOR DISCONNECTED
//	//MOVE THIS PRINTF TO THE HZFLAG IF STATEMENT
//	//printf("Ticks:, %li\n\r",encoderCounts(0));
//	if(((~PIND) & (1<<PIND0))!=0)
//	{
//		setSpeed = 0;
//		//printf("setSpeed: %i\n\r",setSpeed);
//		_delay_ms(50);
//	}
//
//	if(((~PIND) & (1<<PIND1))!=0)
//	{
//		setSpeed = 1023;
//		//printf("setSpeed: %i\n\r",setSpeed);
//		_delay_ms(50);
//	}
//
//	if(((~PIND) & (1<<PIND2))!=0)
//	{
//		setSpeed = 512;
//		//printf("setSpeed: %i\n\r",setSpeed);
//		_delay_ms(50);
//	}
//
//	if(((~PIND) & (1<<PIND3))!=0)
//	{
//		setSpeed = -512;
//		//printf("setSpeed: %i\n\r",setSpeed);
//		_delay_ms(50);
//
//	}



}

void TimerInit100Hz()
{
	//Timer Setup
	TIMSK0= (0<<OCIE0B)|
			(1<<OCIE0A)| //Enable compare A interrupt
			(0<<TOIE0);

	TCCR0A= (0<<COM0A1)|
			(0<<COM0A0)|
			(0<<COM0B1)|
			(0<<COM0B0)|
			(1<<WGM01)|
			(0<<WGM00);

	TCCR0B =(0<<FOC0A)|
			(0<<FOC0B)|
			(0<<WGM02)|
			(1<<CS02)|
			(0<<CS01)| //set clock divide to /1024
			(1<<CS00);
	OCR0A = 179; //the compare register not supposed to be binary
	sei(); //enable global interupts
}

/**
 * @brief interrupt to fire 100Hz
 */
ISR(TIMER0_COMPA_vect)
{
	HzFlag = 1;
	counter ++;
}


int updatePID(int desiredValue, int motor)
{
	if(motor == SHOULDER_MOTOR)
	{
		if(oldValShoulder == -999){
			oldValShoulder = desiredValue;
		}
		if(oldValShoulder != desiredValue){
			oldValShoulder = desiredValue;
			integrationSumShoulder = 0;
		}
		currentVal = adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL));
		error = desiredValue - currentVal;
		integrationSumShoulder += (error/10);
		diffErr = lastPosShoulder-currentVal;
		if(error < 5 && error > -5) error = 0;
		lastPIDOutputShoulder= (int) KpShoulder*error + KiShoulder*integrationSumShoulder + KdShoulder*diffErr;


		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
		lastPosShoulder = currentVal;
		return (int) lastPIDOutputShoulder;
	}
	if(motor == ELBOW_MOTOR)
	{
		if(oldValElbow == -999){
			oldValElbow = desiredValue;
		}
		if(oldValElbow != desiredValue){
			oldValElbow = desiredValue;
			integrationSumElbow = 0;
		}
		currentVal = adcToDegreesArm2(getADC(ELBOW_MOTOR_ADC_CHANNEL));
		error = desiredValue - currentVal;
		integrationSumElbow += (error/10);
		diffErr = lastPosElbow-currentVal;
		if(error < 5 && error > -5) error = 0;
		lastPIDOutputElbow= (int) KpElbow*error + KiElbow*integrationSumElbow + KdElbow*diffErr;


		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
		lastPosShoulder = currentVal;
		return (int) lastPIDOutputElbow;
	}
	return 0;
}
