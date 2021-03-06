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
double adcToDegreesArm2(short adcRead);
int adcToVolts(int adcRead);

#define ADCVERTARM1 640 //ADC value from shoulder pot when are is vertical
#define ADCHORZARM1	200 //ADC value from shoulder pot when are is horizontal

#define ADCHORZPOSARM2 986 //ADC value from shoulder pot when are is vertical
#define ADCHORZNEGARM2	225 //ADC value from shoulder pot when are is horizontal

#define CurSens0 0
#define CurSens1 1
#define Pot0     2
#define Pot1     3



#endif /* LIBRARIES_ADC_H_ */
