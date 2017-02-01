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

int DAC0Val = 0;
int DAC1Val = 1023;
int direction =1;
volatile int hzFlag = 0;
volatile int counter=0;
int integrationSumShoulder = 0;
double integrationSumElbow = 0;
double Kp = 75;
double Ki = 0.01;
double Kd = 16;
int lastPosShoulder = 0;
int lastPosElbow = 0;
int lastPIDOutputShoulder = 0;
int lastPIDOutputElbow = 0;
volatile unsigned int timer = 0;
int DACValue = 1023;
int ADCValue =0;

volatile int counter2=0;
double degreesAAA =0;

int currentVal;
int oldVal = -999;
int error;
int diffErr;

int desiredValue = 0;


void Lab2AInit()
{
	initADC(2);
	initSPI();
	TimerInit100Hz();
	DDRC = (1<<DDA0);
	DDRC = (0<<DDC4|0<<DDC5|0<<DDC6|0<<DDC7);
	PORTC = 0;

	//intiDAC();

}

void Lab2ALoop()
{


	if(hzFlag == 1)
	{
		updatePID(desiredValue,SHOULDER_MOTOR);
		printf("%i,%i,%i,%i\n", desiredValue,currentVal,lastPIDOutputShoulder,ADCtoMillamps(getADC(0)));
	}



	if(counter>5)
	{
		counter=0;
		driveMotor(SHOULDER_MOTOR,lastPIDOutputShoulder);
	}



	if(~PINC & 0b10000) //if PORT B0 is low change value
	{
		desiredValue = 0;

	}

	if(~PINC & 0b100000) //if PORT B1 is low change value
	{
		desiredValue = 30;
	}

	if(~PINC & 0b1000000) //if PORT B2 is low change value
	{
		desiredValue = 60;
	}

	if(~PINC & 0b10000000) //if PORT B3 is low start value
	{

		desiredValue = 90;
	}
}

int updatePID(int desiredValue, int motor)
{
	if(motor == SHOULDER_MOTOR)
	{
		if(oldVal == -999){
			oldVal = desiredValue;
		}
		if(oldVal != desiredValue){
			oldVal = desiredValue;
			integrationSumShoulder = 0;
		}
		currentVal = adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL));
		error = desiredValue - currentVal;
		integrationSumShoulder += (error/10);
		diffErr = lastPosShoulder-currentVal;
		if(error < 5 && error > -5) error = 0;
		lastPIDOutputShoulder= (int) Kp*error + Ki*integrationSumShoulder + Kd*diffErr;


		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
		lastPosShoulder = currentVal;
		return (int) lastPIDOutputShoulder;
	}
/*
	if(motor == ELBOW_MOTOR)
	{
		currentVal = getADC(ELBOW_MOTOR_ADC_CHANNEL);
		error = desiredValue - currentVal;
		integrationSumElbow += error;
		lastPIDOutputElbow=Kp;//*error+Ki*integrationSumElbow+Kd*(lastPIDOutputShoulder-currentVal);
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

