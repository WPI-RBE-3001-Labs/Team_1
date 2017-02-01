/*
 * ADC.h
 *
 *  Created on: Jan 31, 2017
 *      Author: Mike
 */

#ifndef LIBRARIES_ADC_H_
#define LIBRARIES_ADC_H_

double degreesToADCArm1(short degrees);
int ADCtoMillamps(int adcRead);
double adcToDegreesArm1(short adcRead);
double adcToVolts(short adcRead);

#define ADCVERTARM1 640 //ADC value from shoulder pot when are is vertical
#define ADCHORZARM1	224 //ADC value from shoulder pot when are is horizontal

#endif /* LIBRARIES_ADC_H_ */
