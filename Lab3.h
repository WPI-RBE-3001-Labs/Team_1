/*
 * Lab3.h
 *
 *  Created on: Feb 13, 2017
 *      Author: Mike
 */

#ifndef LAB3_H_
#define LAB3_H_

void Lab3Init();
void Lab3Loop();
void TimerInit100Hz();
int updatePID(int desiredValue, int motor);

#endif /* LAB3_H_ */
