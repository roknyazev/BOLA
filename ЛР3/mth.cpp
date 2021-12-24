//
// Created by Князев Роман Олегович on 24.12.2021.
//
#include "cmath"
#include "iostream"

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

void vec3_matrix33_mult(double *vec[], const double *matrix)
{
	*vec[0] = *vec[0] * matrix[0] + *vec[1] * matrix[1] + *vec[2] * matrix[2];
	*vec[1] = *vec[0] * matrix[3] + *vec[1] * matrix[4] + *vec[2] * matrix[5];
	*vec[2] = *vec[0] * matrix[6] + *vec[1] * matrix[7] + *vec[2] * matrix[8];
}


void decline(double **a, const double *b , double angle)
{
	double x = *a[0];
	double y = *a[1];
	double z = *a[2];

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
	*a[0] = x;
	*a[1] = y;
	*a[2] = z;
}
