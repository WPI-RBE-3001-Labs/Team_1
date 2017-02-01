/*
 * Lab2A.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mphopkins
 */

#ifndef LAB2A_H_
#define LAB2A_H_

void Lab2AInit(void);
void Lab2ALoop(void);
void initADC(int channel);
unsigned short getADC(int channel);
double adcToDegreesArm1(short adcRead);
double adcToVolts(short adcRead);
void showTriangleWave(void);
void TimerInit100Hz(void);
void driveMotor(int speed,int motor);
int updatePID(double desiredValue, int motor);


#endif /* LAB2A_H_ */
