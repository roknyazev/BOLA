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

struct params
{
	vector coordinates;
	vector V;
	double g;
	double dt;
	int index;
};

void asp(params &p)
{
	vector coordinates(3) ;
	vector V(3) ;
	double g = p.g;
	double dt = p.dt;
	int index = p.index;

	coordinates[0] = p.coordinates[0];
	coordinates[1] = p.coordinates[1];
	coordinates[2] = p.coordinates[2];
	V[0] = p.V[0];
	V[1] = p.V[1];
	V[2] = p.V[2];
	std::string filename = "../../../sfm/res_asp";
	filename += std::to_string(index);
	filename += ".scv";
	FILE *res = fopen(filename.c_str(), "w");

	fprintf(res, "x,y,z\n");
	while (coordinates[2] > 0)
	{
		V[2] = V[2] - g * dt;
		coordinates[2] = coordinates[2] + V[2]*dt - g * pow(dt, 2)  / 2;
		coordinates[0] = coordinates[0] + V[0] * dt;
		coordinates[1] = coordinates[1] + V[1] * dt;
		fprintf(res, "%f,%f,%f\n", coordinates[0], coordinates[1], coordinates[2]);
	}
	fclose(res);
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

	fprintf(res, "x,y,z,pitch,az,target_az,aim_pitch_target,aim_roll_target,aim_pitch,aim_roll\n");

	vector tmp(3);
	double dist;

	vector sight_vec;
	double aim_pitch = 0;
	double aim_pitch_feedback = 0;
	double aim_pitch_target;
	double aim_roll = 0;
	double aim_roll_feedback = 0;
	double aim_roll_target;
	double A;
	double g = 9.81;
	auto *par = new params();
	std::thread	*thread;
	int asp_index = 0;
	bool drop = false;
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

		if (pitch <= 0.1)
		{
			roll = az_control(roll, target_az);
			az_u = roll - az_feedback;
			az_feedback = isodromic(dt, 1, 0, az_feedback, az_u);
			az = az_feedback;
		}

		// поворот на азимут и крен вектора скорости
		tmp[0] = cos(az);
		tmp[1] = sin(az);
		tmp[2] = 0;
		vel = tmp * v0;
		vel = decline(vel, oz, pitch);

		xyz = xyz + (vel * dt);

		sight_vec = *checkpoint - xyz;
		aim_pitch_target = acos((sight_vec * oz) / sight_vec.length()) - (M_PI / 2);
		tmp = cross_product(vel, oz);
		aim_roll_target = acos((sight_vec * tmp) / (sight_vec.length() * tmp.length())) - (M_PI / 2);

		aim_pitch_feedback = isodromic(dt, 1, 0, aim_pitch_feedback, aim_pitch_target - aim_pitch);
		aim_pitch = aim_pitch_feedback;
		if (aim_pitch > M_PI / 2)
			aim_pitch = M_PI;

		aim_roll_feedback = isodromic(dt, 1, 0, aim_roll_feedback, aim_roll_target - aim_roll);
		aim_roll = aim_roll_feedback;
		if (aim_roll > M_PI / 4)
			aim_roll = M_PI / 4;
		if (aim_roll < - M_PI / 4)
			aim_roll = - M_PI / 4;

		dist = calcDist(xyz, *checkpoint);
		A = sqrt(pow(vel[0], 2) + pow(vel[1], 2)) * (sqrt(2 * xyz[2] / g));
		if (A < dist + 5 && A > dist - 5 && !drop && !end)
		{
			std::cout << asp_index << std::endl;
			asp_index++;
			par->coordinates = xyz;
			par->V = vel;
			par->dt = dt;
			par->g = g;
			par->index = asp_index;
			thread = new std::thread(asp, std::ref(*par));
			thread->detach();
			drop = true;
		}


		if (end && dist < 10)
			return;
		if (dist < 10)
		{
			checkpoint++;
			drop = false;
		}

		if (checkpoint == checkpoints->end())
		{
			checkpoint--;
			(*checkpoint)[0] = 0;
			(*checkpoint)[1] = 0;
			(*checkpoint)[2] = 0;
			end = true;
		}

		//std::cout << dist << std::endl;
		fprintf(res, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
				xyz[0],
				xyz[1],
				xyz[2],
				pitch,
				rad2deg(az),
				rad2deg(target_az),
				rad2deg(aim_pitch_target),
				rad2deg(aim_roll_target),
				rad2deg(aim_pitch),
				rad2deg(aim_roll));
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