//
// Created by Князев Роман Олегович on 23.12.2021.
//

#include "Aircraft.h"

Aircraft::Aircraft()
{
}

Aircraft::Aircraft(double lat, double lon, double alt, double v0, std::list<vector> *checkpoints) :
state()
{
	this->v0 = v0;

	xyz = vector(3);
	xyz[0] = 0;
	xyz[1] = 0;
	xyz[2] = 0;

	vel = vector(3);
	vel[0] = v0;
	vel[1] = 0;
	vel[2] = 0;

	az = 0;
	pitch = 0;
	roll = 0;
	start = false;

	ox = vector(3);
	ox[0] = 1;
	ox[1] = 0;
	ox[2] = 0;

	oy = vector(3);
	oy[0] = 0;
	oy[1] = 1;
	oy[2] = 0;

	oz = vector(3);
	oz[0] = 0;
	oz[1] = 0;
	oz[2] = 1;
	this->checkpoints = checkpoints;
	end = false;
}

double isodromic(double dt, double k1, double k2, double x0, double fx)
{
	return (x0 + (k1 * fx * dt)) + k2 * fx;
}

double rad2deg(double angle)
{
	return angle * 180 / M_PI;
}

double deg2rad(double angle)
{
	return angle * M_PI / 180;
}

void Aircraft::fly()
{
	std::cout << "Start" << std::endl;
	double dt = 1e-3;
	double time = 0;

	auto checkpoint = checkpoints->begin();

	double pitch_u;
	double pitch_feedback = 0;

	double target_az;

	double az_u;
	double az_feedback = 0;


	FILE *res = fopen("../../../sfm/res.csv", "w");

	fprintf(res, "x,y,z,pitch,az,target_az\n");

	vector tmp(3);
	double dist;
	while (true)
	{
		// расчет тангажа
		pitch = pitch_feedback;
		pitch_u = pitch_control(time, 0) - pitch_feedback;
		pitch_feedback = isodromic(dt, 1, 0, pitch_feedback, pitch_u);
		pitch = (pitch_feedback - pitch) / dt;

		// расчет азимута
		tmp = xyz;
		tmp[2] = 0;
		tmp = (*checkpoint - tmp) - ox;
		target_az = atan2(tmp[1], tmp[0]);

		roll = az_control(roll, target_az);
		az_u = roll - az_feedback;
		az_feedback = isodromic(dt, 1, 0, az_feedback, az_u);
		az = az_feedback;

		// поворот на азимут и крен вектора скорости
		tmp[0] = cos(az);
		tmp[1] = sin(az);
		tmp[2] = 0;
		vel = tmp * v0;
		vel = decline(vel, oz, pitch);

		xyz = xyz + (vel * dt);

		dist = calcDist(xyz, *checkpoint);
		if (end && dist < 10)
			return;
		if (dist < 10)
			checkpoint++;
		if (checkpoint == checkpoints->end())
		{
			checkpoint--;
			std::cout << "123" << std::endl;
			(*checkpoint)[0] = 0;
			(*checkpoint)[1] = 0;
			(*checkpoint)[2] = 0;
			std::cout << "123" << std::endl;
			end = true;
		}

		std::cout << dist << std::endl;
		fprintf(res, "%f,%f,%f,%f,%f,%f\n", xyz[0], xyz[1], xyz[2], pitch, rad2deg(az), rad2deg(target_az));
		//std::this_thread::sleep_for(0.1ms);
		time += dt;
	}
}

Aircraft::~Aircraft()
{

}

double az_control(double p, double target_p)
{
	if (target_p > p)
		p += 0.001;
	else
		p -= 0.001;
	return p;
}

double pitch_control(double t, double y)
{
	(void)y;
	if (t < 10)
		return t * 5;
	return 50;
}

void start_aircraft(Aircraft &aircraft)
{
	while (!aircraft.start)
		std::this_thread::sleep_for(1ms);
	aircraft.fly();
}