/*
 * kinematics.h
 *
 *  Created on: Feb 1, 2017
 *      Author: Mike
 */

#ifndef LIBRARIES_KINEMATICS_H_
#define LIBRARIES_KINEMATICS_H_

#define LENGTH1 152
#define LENGTH2 121

double xyToTheta1(double x, double y,double theata2);
double xyToTheta2(double x, double y);
int signTheta2(double x,double y);

double getX(double theta1, double theta2);
double getY(double theta1, double theta2);

#endif /* LIBRARIES_KINEMATICS_H_ */
