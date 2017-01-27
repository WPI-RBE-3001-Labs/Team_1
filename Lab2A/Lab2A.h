/*
 * Lab2A.h
 *
 *  Created on: Jan 27, 2017
 *      Author: mphopkins
 */

#ifndef LAB2A_H_
#define LAB2A_H_

#define ADCVERTARM1 570
#define ADCHORZARM1	190

void Lab2AInit(void);
void Lab2ALoop(void);
void initADC(int channel);
unsigned short getADC(int channel);
double adcToDegreesArm1(short adcRead);
double adcToVolts(short adcRead);

#endif /* LAB2A_H_ */
