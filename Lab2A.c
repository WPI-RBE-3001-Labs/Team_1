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

int DAC0Val = 0;
int DAC1Val = 1023;
int direction =1;
int hzFlag = 0;
double integrationSumShoulder = 0;
double integrationSumElbow = 0;
double Kp = 0;
double Ki = 0;
double Kd = 0;
int lastPIDOutputShoulder = 0;
int lastPIDOutputElbow = 0;

void Lab2AInit()
{
	//initADC(2);
	initSPI();
	setDAC(0,600);
	//TimerInit100Hz();
	//DDRC = (1<<DDA0);
	//PORTC = 0;
}

void Lab2ALoop()
{
	setDAC(0,1023);

}

int updatePID(double desiredValue, int motor)
{
	int currentVal;
	double error;

	if(motor == SHOULDER_MOTOR)
	{
		currentVal = getADC(SHOLDER_MOTOR_ADC_CHANNEL);
		error = desiredValue - currentVal;
		integrationSumShoulder += error;
		lastPIDOutputShoulder=Kp*error+Ki*integrationSumShoulder+Kd*lastPIDOutputShoulder;
		return (int) lastPIDOutputShoulder;
	}

	if(motor == ELBOW_MOTOR)
	{
		currentVal = getADC(ELBOW_MOTOR_ADC_CHANNEL);
		error = desiredValue - currentVal;
		integrationSumElbow += error;
		lastPIDOutputElbow=Kp*error+Ki*integrationSumElbow+Kd*lastPIDOutputElbow;
		return (int) lastPIDOutputElbow;
	}
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

