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

void showTriangleWave(void);
void TimerInit100Hz(void);
void driveMotor(int speed,int motor);
int updatePID(int desiredValue, int motor);

#define STEP_TEST 0
#define TRIANGLE_DRAW 1
#define FANCY_DRAW_TRIANGLE 2

#endif /* LAB2A_H_ */
