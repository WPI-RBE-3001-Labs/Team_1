/*
 * accel.h
 *
 *  Created on: Feb 13, 2017
 *      Author: Mike
 */

#ifndef LIBRARIES_ACCEL_H_
#define LIBRARIES_ACCEL_H_

#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

signed int GetAccelerationH48C(int axis);

#define ACC_SS   PORTCbits._P0
#define ACC_DDR   DDRCbits._P0

#endif /* LIBRARIES_ACCEL_H_ */
