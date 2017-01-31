/*
 * Lab2A.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mphopkins
 */

#ifndef LAB2A_H_
#define LAB2A_H_

#define ADCVERTARM1 570 //ADC value from shoulder pot when are is vertical
#define ADCHORZARM1	190 //ADC value from shoulder pot when are is horizontal

#define SHOULDER_MOTOR 0
#define ELBOW_MOTOR 1

#define SHOLDER_MOTOR_ADC_CHANNEL 0
#define ELBOW_MOTOR_ADC_CHANNEL 1


void Lab2AInit(void);
void Lab2ALoop(void);
void initADC(int channel);
unsigned short getADC(int channel);
double adcToDegreesArm1(short adcRead);
double adcToVolts(short adcRead);
void showTriangleWave(void);
void TimerInit100Hz(void);
void driveMotor(int speed,int motor);

#endif /* LAB2A_H_ */
