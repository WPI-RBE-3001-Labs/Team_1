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
<<<<<<< HEAD
double adcToDegreesArm1(short adcRead);
double adcToDegreesArm2(short adcRead);
double adcToVolts(short adcRead);
=======
int adcToDegreesArm1(short adcRead);
int adcToVolts(int adcRead);
>>>>>>> branch 'master' of https://github.com/WPI-RBE-3001-Labs/Team_1.git

#define ADCVERTARM1 640 //ADC value from shoulder pot when are is vertical
#define ADCHORZARM1	200 //ADC value from shoulder pot when are is horizontal

#define ADCHORZPOSARM2 986 //ADC value from shoulder pot when are is vertical
#define ADCHORZNEGARM2	225 //ADC value from shoulder pot when are is horizontal

#endif /* LIBRARIES_ADC_H_ */
