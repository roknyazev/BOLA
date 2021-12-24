//
// Created by Князев Роман Олегович on 24.12.2021.
//

#ifndef LW1_MTH_H
#define LW1_MTH_H
#include "cmath"
double rungeKutta(double t0,
				  double t,
				  double y0,
				  double step,
				  double (*dydt)(double x, double y));

void vec3_matrix33_mult(double *vec[], const double *matrix);
void decline(double **a, const double *b , double angle);

#endif //LW1_MTH_H
