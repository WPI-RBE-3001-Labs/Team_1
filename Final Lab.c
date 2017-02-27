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
#include "Libraries/rangeSensor.h"
#include "Libraries/kinematics.h"

#define gripperPin 1
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
double desiredValueShoulder = 0;
double desiredValueElbow = 0;
int currentVal;
int oldValShoulder = -999;
int oldValElbow = -999;
int error;
int diffErr;
int counter = 0;
float theta1,theta2;

void FinalLabInit() {
	initSPI();
	initEncoder(0);
	initADC(SHOULDER_MOTOR_ADC_CHANNEL);
	initADC(ELBOW_MOTOR_ADC_CHANNEL);
	TimerInit100Hz();
	initADC(7);
	setServo(0,300);//start belt
	setServo(1,10);
	theta1=70;
	theta2=0;

}

signed int xG = 0;
signed int yG = 0;
signed int zG = 0;
signed long total = 0;
signed long temp = 0;
int setSpeed = 0;
int tickTemp = 0;
double adcShoulder = 0;
long encoderTotal = 0;

enum States {
	waitingForStart,    //Waiting for the range sensor to read a block
	movingToBlockPos,   //moving the arm to the position where the block will be
	waitingForBlock,    //waiting in position for the block to reach it
	closingOnBlock,     //closing the servo
	movingToHor,        //moving to full horizontal for the current sensor bit
	movingToVert,       //moving to straight vertical while reading sensor
	movingLight,        //if light block, move to this location
	movingHeavy,        //if heavy block, move here
	releasingBlock      //release servo
};

int state = waitingForStart;
int blockDist;
int desiredX = 8;
int desiredY = 5;
int currentX = 0;
int currentY = 0;

int maxCurRead = 0;
int thisCurRead = 0;

int primaryRangeSensorChannel = 7;
int secondaryRangeSensorChannel = 6;
/*
 * TODO commands are given in angles, but error checking done in pos. Change to one or the other or adapt
 * TODO errors need to be determined
 * TODO checking pos needs to be done in each moving state. Right now no code updated currentX or currentY
 */

void FinalLabLoop() { //NON BLOCKING. NO WHILE or long FOR loops!
	//printf("desiredX: %i,desiredY: %i\n\r",desiredX,desiredY);
	if(HzFlag==1)
	{
		//theta2 = xyToTheta2(desiredX, desiredY);
		//theta2 *= signTheta2(desiredX, desiredY);
		//theta1 = xyToTheta1(desiredX, desiredY,theta2);
		//printf("theta2:%f, theta1:%f",theta2,theta1);
		//printf("LastPosShoulder: %f,LastPosElbow: %f\n\r",adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL)),adcToDegreesArm2(getADC(ELBOW_MOTOR_ADC_CHANNEL)));
		updatePID(theta1, SHOULDER_MOTOR);
		updatePID(theta2, ELBOW_MOTOR);
	}

	if(counter >10)
	{
		driveMotor(SHOULDER_MOTOR, lastPIDOutputShoulder); //Move the motors each iteration
		driveMotor(ELBOW_MOTOR, -1*lastPIDOutputElbow);
		//printf("Motor Output Elbow:%i,Motor Output Shoulder:%i\n\r",lastPIDOutputElbow,lastPIDOutputShoulder);
		counter=0;
	}
	switch (state) {

	case waitingForStart:
		printf("Waiting For Start\n\r");
		if ((blockDist = getIRInches(primaryRangeSensorChannel)) < 6) {
			//TODO inverse kinematics to set the desired angles
			//desiredX = 8;
			//desiredY = 5;
			theta1=60;
			theta2=-75;
			state = waitingForBlock;
		}

		break;
/*
	case movingToBlockPos:
		printf("Moving to Block Pos\n\r");
		currentX = (int) adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL));
		currentY = (int) adcToDegreesArm1(getADC(ELBOW_MOTOR_ADC_CHANNEL));

		if (isFinalPos()) {
			state = waitingForBlock;
		}
		break;
*/
	case waitingForBlock:
		printf("Waiting For Block. Range Sensor: %f\n\r",getIRInches(secondaryRangeSensorChannel));
		if (getIRInches(secondaryRangeSensorChannel) < 6) {
			state = closingOnBlock;
		}

		break;

	case closingOnBlock:
		//TODO Update PID to lower the rest of the way
		//TODONE also close servo
		theta1=30;
		theta2=-75;
		setServo(1,10); //closes servo. Gripper should be plugged into Servo1
		//if ((currentX - desiredX) < 3 && (currentY - desiredY) < 3) { //TODO tune error range
		//	state = movingToHor;
		//}
		state=movingToHor;
		break;

	case movingToHor:

		if (isFinalPos()) {
			//state = movingToVert;
			maxCurRead = 0;
		}

		break;
	case movingToVert:

		if ((thisCurRead = ADCtoMillamps(getADC(0))) > maxCurRead) {
			maxCurRead = thisCurRead;
		}

		if (isFinalPos()) {
			if (maxCurRead > 55) { //TODO find actual difference in current to determine which block
				state = movingHeavy;
			} else {
				state = movingLight;
			}
		}

		break;
	case movingLight:
		if (isFinalPos()) {
			state = releasingBlock;
		}

		break;
	case movingHeavy:

		if (isFinalPos()) {
			state = releasingBlock;
		}
		break;
	case releasingBlock:

		//TODONE release Servo
		setServo(gripperPin,100);
		state = waitingForStart;
		break;
	}

}

void TimerInit100Hz() {
	//Timer Setup
	TIMSK0 = (0 << OCIE0B) | (1 << OCIE0A) | //Enable compare A interrupt
			(0 << TOIE0);

	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0)
			| (1 << WGM01) | (0 << WGM00);

	TCCR0B = (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (1 << CS02)
			| (0 << CS01) | //set clock divide to /1024
			(1 << CS00);
	OCR0A = 179; //the compare register not supposed to be binary
	sei();
	//enable global interupts
}

/**
 * @brief interrupt to fire 100Hz
 */
ISR(TIMER0_COMPA_vect) {
	HzFlag = 1;
	counter++;
}
int updatePID(int desiredValue, int motor) {

	if (motor == SHOULDER_MOTOR) {
		if (oldValShoulder == -999) {
			oldValShoulder = desiredValue;
		}
		if (oldValShoulder != desiredValue) {
			oldValShoulder = desiredValue;
			integrationSumShoulder = 0;
		}
		currentVal = adcToDegreesArm1(getADC(SHOULDER_MOTOR_ADC_CHANNEL));
		error = desiredValue - currentVal;
		integrationSumShoulder += (error / 10);
		diffErr = lastPosShoulder - currentVal;
		if (error < 5 && error > -5)
			error = 0;
		lastPIDOutputShoulder = (int) KpShoulder * error
				+ KiShoulder * integrationSumShoulder + KdShoulder * diffErr;

		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
		lastPosShoulder = currentVal;
		return (int) lastPIDOutputShoulder;
	}
	if (motor == ELBOW_MOTOR) {
		if (oldValElbow == -999) {
			oldValElbow = desiredValue;
		}
		if (oldValElbow != desiredValue) {
			oldValElbow = desiredValue;
			integrationSumElbow = 0;
		}
		currentVal = adcToDegreesArm2(getADC(ELBOW_MOTOR_ADC_CHANNEL));
		error = desiredValue - currentVal;
		integrationSumElbow += (error / 10);
		diffErr = lastPosElbow - currentVal;
		if (error < 5 && error > -5)
			error = 0;
		lastPIDOutputElbow = (int) KpElbow * error
				+ KiElbow * integrationSumElbow + KdElbow * diffErr;

		//printf("Err %i, INT %i, DER %i, OUT %i\n\r",error,integrationSumShoulder,diffErr,(int)lastPIDOutputShoulder );
		lastPosShoulder = currentVal;
		return (int) lastPIDOutputElbow;
	}
	return 0;
}

int isFinalPos(){
	int errorRange = 30;

	int shoulder = 0;
	if(lastPIDOutputShoulder < errorRange && lastPIDOutputShoulder > -1*errorRange){
		shoulder = 1;
	}

	int elbow = 0;
	if(lastPIDOutputElbow < errorRange && lastPIDOutputElbow > -1*errorRange){
			elbow = 1;
		}

	return (shoulder * elbow);

}
