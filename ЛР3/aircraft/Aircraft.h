//
// Created by Князев Роман Олегович on 23.12.2021.
//

#ifndef LW1_AIRCRAFT_H
#define LW1_AIRCRAFT_H
#include <random>
#include "../Timer.h"
#include "../transform.h"
#include "../mth.h"

class Aircraft
{
private:

	double x;
	double y;
	double v;

	double lat;
	double lon;
	double alt;
	double az; //курс истинный
	double pitch; //тангаж
	double roll; //крен

	double a_x;
	double a_y;
	double a_z;

	double rotation_matrix[9];
	double *vel; // скорость север, скорость восток, вертикальная скорость
private:
	double state[12];


public:
	Aircraft(double lat, double lon, double alt, double az, double v0);
	~Aircraft();
	Aircraft();
	[[noreturn]] void fly();
	bool start;
	const double *get_state() const;
	void rotate(double d_pitch, double azimuth);
};

double pitch_control(double t, double y);
void start_aircraft(Aircraft &aircraft);

#endif //LW1_AIRCRAFT_H
