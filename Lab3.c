///*
// * Lab3.c
// *
// *  Created on: Feb 13, 2017
// *      Author: Mike
// */
//
//
//#include "RBELib/RBELib.h"
////For use of abs()
//#include <stdlib.h>
//#include "Libraries/accel.h"
//#include "Libraries/encoders.h"
//#include "Lab3.h"
//#include "Libraries/motors.h"
//#include "Libraries/ADC.h"
//#include "Libraries/DAC.h"
//#include "Libraries/rangeSensor.h"
//
////ISR Variables
//volatile int HzFlag = 0;
//
////PID Vaiables
//int integrationSumShoulder = 0;
//double integrationSumElbow = 0;
//
//double KpShoulder = 45;
//double KiShoulder = 0.03;
//double KdShoulder = 0;
//
//double KpElbow = 20;
//double KiElbow = 0.01;
//double KdElbow = 0;
//
//int lastPosShoulder = 0;
//int lastPosElbow = 0;
//int lastPIDOutputShoulder = 0;
//int lastPIDOutputElbow = 0;
//volatile unsigned int timer = 0;
//double desiredValueShoulder = 0;
//double desiredValueElbow=0;
//int currentVal;
//int oldValShoulder = -999;
//int oldValElbow = -999;
//int error;
//int diffErr;
//int counter=0;
//
//
//
//
//void Lab3Init()
//{
//	initSPI();
//	initEncoder(0);
//	initADC(SHOULDER_MOTOR_ADC_CHANNEL);
//	initADC(ELBOW_MOTOR_ADC_CHANNEL);
//	TimerInit100Hz();
//	initADC(7);
//	setServo(0,0);
//
//	//Configure Buttons by setting pins to input
//	DDRD &= ~( (1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3) );
//
//	//printf("Desired Position: (%i,%i)  Theta1: %f  Theta2: %f\n\r",x,y,xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y)),signTheta2(x,y)*xyToTheta2(x,y));
//	//	desiredValueShoulder = xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y));
//	//	desiredValueElbow = signTheta2(x,y)*xyToTheta2(x,y);
//
//	//printf("Lab3.c\r\n");
//}
//
//signed int xG=0;
//signed int yG=0;
//signed int zG=0;
//signed long total =0;
//signed long temp=0;
//int setSpeed=0;
//int tickTemp = 0;
//double adcShoulder = 0;
//long encoderTotal = 0;
//int i = 0;
//void Lab3Loop()
//{
//	printf("Inches: %f\n\r",getIRmm(7));
//	setServo(0,0);
//	i+=10;
//	if(i>250)
//	{
//		i=0;
//	}
//	_delay_ms(200);
//
//	/*
//	xG = GetAccelerationH48C(0);
//	yG = GetAccelerationH48C(1);
//	zG = GetAccelerationH48C(2);
//	if(HzFlag == 1)
//	{
//		tickTemp = encoderCounts(0);
//		encoderTotal += tickTemp;
//		adcShoulder = adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL));
//
//		if( (error < 5 && error > -5) && (adcShoulder > -5 && adcShoulder < 2)){
//			encoderTotal = 0;
//		}
//
//
//		printf("%d, %d, %d\r\n", xG, yG, zG);
//		updatePID(desiredValueShoulder,SHOULDER_MOTOR);
//		HzFlag = 0;
//	}
//
//	if(counter>5)
//	{
//		counter=0;
//		driveMotor(SHOULDER_MOTOR,lastPIDOutputShoulder);
//		//printf("Desired Value Shoulder: %.0f, Current Value: %i,Shoulder PID: %i\n\r", desiredValueShoulder,currentVal,lastPIDOutputShoulder);
//
//	}
//
//	if( ((~PIND) & (1<<PIND0)) )
//	{
//		desiredValueShoulder = 0;
//		_delay_ms(20);
//	}
//
//	if(((~PIND) & (1<<PIND1)))
//	{
//		desiredValueShoulder = 90;
//		_delay_ms(20);
//	}
//	if(((~PIND) & (1<<PIND2)))
//	{
//		encoderTotal = 0;
//	}
//
//	//driveMotor(SHOULDER_MOTOR,setSpeed);
//
//	//THIS IS FOR THE DATA COLLECTION WITH MOTOR DISCONNECTED
//	//MOVE THIS PRINTF TO THE HZFLAG IF STATEMENT
//	//printf("%li\n\r",tickTemp);
//	//		if(((~PIND) & (1<<PIND0)))
//	//		{
//	//			setSpeed = 0;
//	//			printf("setSpeed set to 0\n\r");
//	//			_delay_ms(50);
//	//		}
//	//
//	//		if(((~PIND) & (1<<PIND1)))
//	//		{
//	//			setSpeed = 1023;
//	//			printf("setSpeed set to 1023\n\r");
//	//			_delay_ms(50);
//	//		}
//	//
//	//		if(((~PIND) & (1<<PIND2)))
//	//		{
//	//			setSpeed = 512;
//	//			printf("setSpeed set to 512\n\r");
//	//			_delay_ms(50);
//	//		}
//	//
//	//		if(((~PIND) & (1<<PIND3)))
//	//		{
//	//			setSpeed = -512;
//	//			printf("setSpeed set to -512\n\r");
//	//			_delay_ms(50);
//	//
//	//		}
//
//
//*/
//}
//
//void TimerInit100Hz()
//{
//	//Timer Setup
//	TIMSK0= (0<<OCIE0B)|
//			(1<<OCIE0A)| //Enable compare A interrupt
//			(0<<TOIE0);
//
//	TCCR0A= (0<<COM0A1)|
//			(0<<COM0A0)|
//			(0<<COM0B1)|
//			(0<<COM0B0)|
//			(1<<WGM01)|
//			(0<<WGM00);
//
//	TCCR0B =(0<<FOC0A)|
//			(0<<FOC0B)|
//			(0<<WGM02)|
//			(1<<CS02)|
//			(0<<CS01)| //set clock divide to /1024
//			(1<<CS00);
//	OCR0A = 179; //the compare register not supposed to be binary
//	sei(); //enable global interupts
//}
//
///**
// * @brief interrupt to fire 100Hz
// */
//ISR(TIMER0_COMPA_vect)
//{
//	HzFlag = 1;
//	counter ++;
//}
//
//
//int updatePID(int desiredValue, int motor)
//{
//
//	if(motor == SHOULDER_MOTOR)
//	{
//		if(oldValShoulder == -999){
//			oldValShoulder = desiredValue;
//		}
//		if(oldValShoulder != desiredValue){
//			oldValShoulder = desiredValue;
//			integrationSumShoulder = 0;
//		}
//		currentVal = adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL));
//		error = desiredValue - currentVal;
//		integrationSumShoulder += (error/10);
//		diffErr = lastPosShoulder-currentVal;
//		if(error < 5 && error > -5) error = 0;
//		lastPIDOutputShoulder= (int) KpShoulder*error + KiShoulder*integrationSumShoulder + KdShoulder*diffErr;
//
//
//		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
//		lastPosShoulder = currentVal;
//		return (int) lastPIDOutputShoulder;
//	}
//	if(motor == ELBOW_MOTOR)
//	{
//		if(oldValElbow == -999){
//			oldValElbow = desiredValue;
//		}
//		if(oldValElbow != desiredValue){
//			oldValElbow = desiredValue;
//			integrationSumElbow = 0;
//		}
//		currentVal = adcToDegreesArm2(getADC(ELBOW_MOTOR_ADC_CHANNEL));
//		error = desiredValue - currentVal;
//		integrationSumElbow += (error/10);
//		diffErr = lastPosElbow-currentVal;
//		if(error < 5 && error > -5) error = 0;
//		lastPIDOutputElbow= (int) KpElbow*error + KiElbow*integrationSumElbow + KdElbow*diffErr;
//
//
//		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
//		lastPosShoulder = currentVal;
//		return (int) lastPIDOutputElbow;
//	}
//	return 0;
//}
