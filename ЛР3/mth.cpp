//
// Created by Князев Роман Олегович on 24.12.2021.
//
#include "cmath"
#include "iostream"
#include "vector"
#include "map"
#include "mth.h"

#define EARTH_RADIUS 6371

double rungeKutta(double t0,
				  double t,
				  double y0,
				  double step,
				  double (*dydt)(double t, double y))
{
	int n = (int)((t - t0) / step);
	double k1, k2, k3, k4;

	double y = y0;
	for (int i=1; i<=n; i++)
	{
		k1 = step * dydt(t0, y);
		k2 = step * dydt(t0 + 0.5 * step, y + 0.5 * k1);
		k3 = step * dydt(t0 + 0.5 * step, y + 0.5 * k2);
		k4 = step * dydt(t0 + step, y + k3);

		y = y + (1.0/6.0)*(k1 + 2*k2 + 2*k3 + k4);

		t0 = t0 + step;
	}
	return y;
}

std::vector<double> vec3_matrix33_mult(const double *vec, const double *matrix)
{
	std::vector<double> res;

	res.resize(3);
	res[0] = vec[0] * matrix[0] + vec[1] * matrix[1] + vec[2] * matrix[2];
	res[1] = vec[0] * matrix[3] + vec[1] * matrix[4] + vec[2] * matrix[5];
	res[2] = vec[0] * matrix[6] + vec[1] * matrix[7] + vec[2] * matrix[8];
	return res;
}


vector decline(vector a, vector b , double angle)
{
	vector res(3);

	double x = a[0];
	double y = a[1];
	double z = a[2];

	double tempX = y*b[2] - z*b[1];
	double tempY = z*b[0] - x*b[2];
	double tempZ = x*b[1] - y*b[0];
	double cX = tempY*z - tempZ*y;
	double cY = tempZ*x - tempX*z;
	double cZ = tempX*y - tempY*x;

// Нормализуем вектора A и C
	double lengthA = sqrt(x*x + y*y + z*z);
	double lengthC = sqrt(cX*cX + cY*cY + cZ*cZ);
	double normAX = x / lengthA;
	double normAY = y / lengthA;
	double normAZ = z / lengthA;
	double normCX = cX / lengthC;
	double normCY = cY / lengthC;
	double normCZ = cZ / lengthC;

// Находим координаты вектора, в локальной системе координат задаваемой векторами A и B.
	double localX = cos(M_PI / 180 * angle);
	double localY = sin(M_PI / 180 * angle);

// Умножаем векторы A и B на скаляры - длины компонентов результатирующего вектора
	double newXA = normAX * localX;
	double newYA = normAY * localX;
	double newZA = normAZ * localX;
	double newXB = normCX * localY;
	double newYB = normCY * localY;
	double newZB = normCZ * localY;

// Складываем полученные векторы
	x = newXA + newXB;
	y = newYA + newYB;
	z = newZA + newZB;

// возвращаем нашему вектору прежнюю длину умножая его на скаляр.
	x *= lengthA;
	y *= lengthA;
	z *= lengthA;


	res[0] = x;
	res[1] = y;
	res[2] = z;
	return res;
}

double calcDist(vector departure, vector destination)
{
//	double ph1 = M_PI / 180 * departure[0];
//	double lm1 = M_PI / 180 * departure[1];
//	double ph2 = M_PI / 180 * destination[0];
//	double lm2 = M_PI / 180 * destination[1];
//
//	double delta_lm = lm2 - lm1;
//	double result;
//
//	double delta_sigma;
//
//	double tmp1 = sqrt(pow((cos(ph2) * sin(delta_lm)), 2) +
//			pow((cos(ph1) * sin(ph2) - sin(ph1) * cos(ph2) * cos(delta_lm)), 2));
//	double tmp2 = sin(ph1) * sin(ph2) + cos(ph1) * cos(ph2) * cos(delta_lm);
//	delta_sigma = atan2(tmp1, tmp2);
//	result = delta_sigma * EARTH_RADIUS;
	std::vector<double> vec1(3);
	std::vector<double> vec2(3);

	vec1[0] = departure[0];
	vec1[1] = departure[1];
	vec1[2] = 0;

	vec2[0] = destination[0];
	vec2[1] = destination[1];
	vec2[2] = 0;

	double result = sqrt(pow(vec1[0] - vec2[0] , 2) + pow(vec1[1] - vec2[1], 2) + pow(vec1[2] - vec2[2], 2));
	return result;
}

std::vector<double> sphere_to_cartesian(double lat, double lon) // из сферичеких в декартовы
{
	std::vector<double> res;

	res.resize(3);
	res[0] = EARTH_RADIUS * cos(M_PI / 180 * lat) * cos(M_PI / 180 * lon);
	res[1] = EARTH_RADIUS * cos(M_PI / 180 * lat) * sin(M_PI / 180 * lon);
	res[2] = EARTH_RADIUS * sin(M_PI / 180 * lat);

	return res;
}

std::vector<double> cartesian_to_sphere(double x, double y, double z) // из декартовых в сферичекие
{
	std::vector<double> res;

	res.resize(3);
	res[0] = asin(z / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
	res[1] = atan2(y, x);
	res[2] = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	return res;
}

double len(std::vector<double> vec)
{
	return sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
}

double scalar_product(std::vector<double> vec1, std::vector<double> vec2)
{
	double result = vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2];
	result /= len(vec1) * len(vec2);
	return result;
}

double get_az(const double departure[2], const double destination[2])
{
	std::vector<double> vec1(3);
	std::vector<double> vec2(3);
	vec1[0] = 1;
	vec1[1] = 0;
	vec1[2] = 0;

	vec2[0] = destination[0] - departure[0];
	vec2[1] = destination[1] - departure[1];
	vec2[2] = 0;

	double result = scalar_product(vec1, vec2);
	return  (acos(result) * 180 / M_PI);
//	std::vector<double> pos1 = sphere_to_cartesian(departure[0], departure[1]);
//	std::vector<double> pos2 = sphere_to_cartesian(destination[0], destination[1]);
//
//	std::vector<double> pos3 = sphere_to_cartesian(departure[0] + 0.1, departure[1]);
//
//
//	std::vector<double> vec1(3);
//	std::vector<double> vec2(3);
//
//	vec1[0] = pos2[0] - pos1[0];
//	vec1[1] = pos2[1] - pos1[1];
//	vec1[2] = pos2[2] - pos1[2];
//
//	vec2[0] = pos3[0] - pos1[0];
//	vec2[1] = pos3[1] - pos1[1];
//	vec2[2] = pos3[2] - pos1[2];
//
//	//std::cout << "1: " << vec2[0] << " " << vec2[1] << " " << vec2[2] << std::endl;
//	double result = vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2];
//	//std::cout << "res: " << result << std::endl;
//	result /= len(vec1) * len(vec2);
//
//
////	std::cout << " : " << vec1[0] / vec2[0] << ' ' << vec1[1]/vec2[1] << ' ' << vec1[2]/vec2[2] << std::endl;
////	std::cout << " : " << vec1[0] / vec2[0] << ' ' << vec1[1]/vec2[1] << ' ' << vec1[2]/vec2[2] << std::endl;
//
//
//
//	std::cout << "angle: " << (acos(result) * 180 / M_PI) << std::endl;
//	return  0;//(acos(result) * 180 / M_PI); FIXME
}

vector	cross_product(vector u, vector v)
{
	vector	p(3);

	p[0] = v[2] * u[1] - v[1] * u[2];
	p[1] = v[0] * u[2] - v[2] * u[0];
	p[2] = v[1] * u[0] - v[0] * u[1];
	return (p);
}


std::vector<double> fromStart2Geogr(std::vector<double> vec, double lon, double lat, std::map <int, double > &table)
{
	std::vector<double> dir(3);
	std::vector<double> tmp(3);
	std::vector<double> tmp2(3);
	std::vector<double>	right(3);
	std::vector<double> up(3);
	double ln_vec = len(vec);


	dir = sphere_to_cartesian(lat, lon);
	tmp = dir;


	vec[0] -= dir[0];
	vec[1] -= dir[1];
	vec[2] -= dir[2];

	dir[0] = 1e-100;
	dir[0] = 1;
	dir[0] = 1e-100;

	dir[0] /= len(dir);
	dir[1] /= len(dir);
	dir[2] /= len(dir);


	vec[0] /= len(vec);
	vec[1] /= len(vec);
	vec[2] /= len(vec);

	up[0] = 1e-100;
	up[1] = 1e-100;
	up[2] = 1;

	//right = cross_product(up, dir);

	right[0] /= len(right);
	right[1] /= len(right);
	right[2] /= len(right);

	//up = cross_product(dir, right);

	up[0] /= len(up);
	up[1] /= len(up);
	up[2] /= len(up);

	vec[0] = vec[0] * right[0] + vec[1] * up[0] + vec[2] * dir[0];
	vec[1] = vec[0] * right[1] + vec[1] * up[1] + vec[2] * dir[1];
	vec[2] = vec[0] * right[2] + vec[1] * up[2] + vec[2] * dir[2];

	vec[0] *= ln_vec;
	vec[1] *= ln_vec;
	vec[2] *= ln_vec;

	dir = tmp;
	std::cout << "x: " << dir[0] << " y: " << dir[1] << std::endl;


	tmp2 = cartesian_to_sphere(dir[0] + vec[0],
							   dir[1] + vec[1],
							   dir[2] + vec[2]);

	std::cout << "x: " << vec[0] << " y: " << vec[1] << std::endl;

	std::cout << "x: " << tmp2[0] / M_PI * 180 << " y: " << tmp2[1] / M_PI * 180 << std::endl;

	std::vector<double> res = { tmp2[0] / M_PI * 180, tmp2[1] / M_PI * 180, vec[1] };
	return res;
}


//double x, z, lambda, dlambda, phi, dphi, psi, dx, dz;
//x = 0; z = 0;
//lambda = lon_rad; //долгота
//phi = lat_rad; //широта
//dx = 111000;
//std::vector<double> tmp = vec;
//
//vec[0] = tmp[1];
//vec[1] = tmp[2];
//vec[2] = tmp[0];
//while ((vec[0] - x) > 300 || (vec[2] - z) > 300)
//{
//psi = atan((vec[2] - z) / (vec[0] - x));
//dlambda = floor(lon_rad) + 1 - lon_rad;
//dphi = floor(lat_rad) + 1 - lat_rad;
//
//
//if (dphi < 0.5)
//dz = 1000 * table[int(floor(lat_rad) + 1)];
//else
//dz = 1000 * table[int(floor(lat_rad))];
//
//if ((vec[0] - x) < dx and (vec[2] - z) < dz)
//{
//lambda = lambda + ((vec[2] - z) / dz);
//phi = phi + ((vec[0] - x) / dx);
//x = x + (vec[0] - x);
//z = z + (vec[2] - z);
//}
//else
//{
//if (dlambda * dz < dphi * dx)
//{
//x = x + dphi * dx;
//z = z + (tan(psi) * (dphi * dx));
//phi = phi + dphi;
//lambda = lambda + ((tan(psi) * (dphi * dx)) / dz);
//}
//else
//{
//z = z + dlambda * dz;
//x = x + (atan(psi) * (dlambda * dz));
//lambda = lambda + dlambda;
//phi = phi + ((atan(psi) * (dlambda * dz)) / dx);
//}
//}
//}