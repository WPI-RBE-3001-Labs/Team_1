/*
 * Lab1_ADCandWaveform.h
 *
 *  Created on: Jan 24, 2017
 *      Author: djcupo
 */

#ifndef LAB_1_LAB1_ADCANDWAVEFORM_H_
#define LAB_1_LAB1_ADCANDWAVEFORM_H_

#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

#define WANTEDSAMPLES (2*250)


void Lab1loop(void);
void Lab1init(void);
void sample(void);

#endif /* LAB_1_LAB1_ADCANDWAVEFORM_H_ */
