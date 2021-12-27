//
// Created by Князев Роман Олегович on 24.12.2021.
//

#ifndef LW1_MTH_H
#define LW1_MTH_H
#include "cmath"
#include "linalg.h"

double rungeKutta(double t0,
				  double t,
				  double y0,
				  double step,
				  double (*dydt)(double x, double y));

std::vector<double> vec3_matrix33_mult(const double *vec, const double *matrix);
double scalar_product(std::vector<double> vec1, std::vector<double> vec2);
double len(std::vector<double> vec);
std::vector<double> decline(const double *a, const double *b , double angle);
double get_az(const double departure[2], const double destination[2]);
std::vector<double> fromStart2Geogr(std::vector<double> vec, double lon_rad, double lat_rad, std::map <int, double > &table);
double calcDist(const double departure[2], const double destination[2]);

#endif //LW1_MTH_H
