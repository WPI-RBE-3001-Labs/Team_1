/*
 * Lab2A.c
 *
 *  Created on: Jan 27, 2017
 *      Author: Mike Hopkins
 */


#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>
#include "Lab2A.h"
#include "Libraries/motors.h"
#include "Libraries/ADC.h"
#include "Libraries/kinematics.h"

int DAC0Val = 0;
int DAC1Val = 1023;
int direction =1;
volatile int hzFlag = 0;
volatile int counter=0;
volatile int counter2=0;

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
int DACValue = 1023;
int ADCValue =0;

int currentVal;
int oldValShoulder = -999;
int oldValElbow = -999;
int error;
int diffErr;

int desiredValue = 0;

int x = 0;
int y=10;
double desiredValueShoulder=0;
double desiredValueElbow=0;
double theta1,theta2;

int buttonFunction = FANCY_DRAW_TRIANGLE;


void Lab2AInit()
{
	initADC(2);
	initSPI();
	TimerInit100Hz();
	DDRC = (1<<DDA0);
	DDRC = (0<<DDC4|0<<DDC5|0<<DDC6|0<<DDC7);
	PORTC = 0;

	//printf("Desired Position: (%i,%i)  Theta1: %f  Theta2: %f\n\r",x,y,xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y)),signTheta2(x,y)*xyToTheta2(x,y));
//	desiredValueShoulder = xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y));
//	desiredValueElbow = signTheta2(x,y)*xyToTheta2(x,y);

	//intiDAC();

}

void Lab2ALoop()
{


	if(hzFlag == 1)
	{
		updatePID(desiredValueShoulder,SHOULDER_MOTOR);
		updatePID(desiredValueElbow,ELBOW_MOTOR);
		//printf("Desired Value Elbow, %f, Current Value,%i,Shoulder PID%i,%i\n\r", desiredValueElbow,currentVal,lastPIDOutputElbow,ADCtoMillamps(getADC(0)));
	}



	if(counter>3)
	{
		counter=0;
		driveMotor(SHOULDER_MOTOR,lastPIDOutputShoulder);
		driveMotor(ELBOW_MOTOR,-1*lastPIDOutputElbow);
		//printf("Desired Value Shoulder: %f, Current Value: %i,Shoulder PID: %i,%i\n\r", desiredValueShoulder,currentVal,lastPIDOutputElbow,ADCtoMillamps(getADC(0)));

	}

	if(buttonFunction == FANCY_DRAW_TRIANGLE)
	{
		if(counter2<500)
		{
			desiredValueShoulder = 2;
			desiredValueElbow = 8;
		}else{
			if(counter2<1000)
			{
				desiredValueShoulder = 31;
				desiredValueElbow = -42;
			}else{
				if(counter2<1500)
				{
					desiredValueShoulder = 58;
					desiredValueElbow = -65;
				}else{
					if(counter2<2000)
					{
						desiredValueShoulder = 44;
						desiredValueElbow = -80;
					}else{
						if(counter2<2500)
						{
							desiredValueShoulder = 46;
							desiredValueElbow = -95;
						}else{
							if(counter2<3000)
							{
								desiredValueShoulder = 28;
								desiredValueElbow = -66;
							}else{
								counter2=0;
							}
						}
					}
				}
			}
		}
	}

	if(~PINC & 0b10000) //if PORT C4 is low change value
	{
		switch(buttonFunction) {

		   case STEP_TEST:
		      desiredValueShoulder = 22;
		      desiredValueElbow = 70;
		   break;

		   case TRIANGLE_DRAW:
			   desiredValueShoulder = 2;
			   desiredValueElbow = 8;
		   break;
		}


	}

	if(~PINC & 0b100000) //if PORT C5 is low change value
	{
		switch(buttonFunction)
		{
			case STEP_TEST:
				desiredValueShoulder = 63;
				desiredValueElbow = -45;
				break;

			case TRIANGLE_DRAW:
				desiredValueShoulder = 58;
				desiredValueElbow = -65;
				break;
		}
	}

	if(~PINC & 0b1000000) //if PORT C6 is low change value
	{
		switch(buttonFunction)
		{
			case STEP_TEST:
				desiredValueShoulder = 30;
				desiredValueElbow = -60;
				break;

			case TRIANGLE_DRAW:
				desiredValueShoulder = 46;
				desiredValueElbow = -95;
				break;
		}

	}

	if(~PINC & 0b10000000) //if PORT C7 is low start value
	{
		switch(buttonFunction)
		{
			case STEP_TEST:
				desiredValueShoulder = 50;
				desiredValueElbow = 85;
				break;

			case TRIANGLE_DRAW:
				break;
		}
	}


	theta1 = adcToDegreesArm1(getADC(2));
	theta2 = adcToDegreesArm2(getADC(3));
	double X = getX(theta1, theta2);
	double Y = getY(theta1, theta2);
	printf("T1:,%06.2f, T2:, %06.2f, X:, %06.2f, Y:, %06.2f\n\r",theta1, theta2, X, Y);
	printf("%06.2f,%06.2f,%06.2f,%06.2f\n",theta1, theta2, X, Y);


//	printf("Desired Position: (%i,%i)  Theta1: %f  Theta2: %f\n\r",x,y,xyToTheta1(x,y,signTheta2(x,y)*xyToTheta2(x,y)),signTheta2(x,y)*xyToTheta2(x,y));
//	printf("Arm Angle, %f ADCValue: %f\n\r", adcToDegreesArm2(degreesAAA),degreesAAA);
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
	/*
	if(motor == ELBOW_MOTOR)
	{
		currentVal = getADC(ELBOW_MOTOR_ADC_CHANNEL);
		error = desiredValue - currentVal;
		integrationSumElbow += error;
		lastPIDOutputElbow=Kp;// *error+Ki*integrationSumElbow+Kd*(lastPIDOutputShoulder-currentVal);
		return (int) lastPIDOutputElbow;
	}
	 */
	return 0;
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
	hzFlag = 1;
	counter++;
	counter2++;
}


/**
 *
 * @brief display 2 triangle waves phase shifted 180 degrees Part 5
 *
 */
void showTriangleWave()
{
	/*This is the code to display 2 triangle waves phase shifted 180 degrees Part 5*/
	setDAC(0,DAC0Val);
	setDAC(1,DAC1Val);
	_delay_ms(10);
	if(direction==1)
	{
		DAC0Val += 3;
		DAC1Val -= 3;

		if(DAC0Val>1024)
		{
			direction =0;
		}
	}else{
		DAC0Val -= 3;
		DAC1Val += 3;
		if(DAC0Val<0)
		{
			direction=1;
		}
	}
}


