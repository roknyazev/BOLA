//
// Created by Князев Роман Олегович on 23.12.2021.
//

#include "Aircraft.h"

Aircraft::Aircraft()
{
}

Aircraft::Aircraft(double lat, double lon, double alt, double az, double v0):
lat(lat),
lon(lon),
alt(alt),

az(az),
v(v0),
pitch(0),
roll(0),

a_x(0),
a_y(0),
a_z(0),

state(),
start(false),
rotation_matrix()
{
	//fixme мютекс
	vel = new double[3];
	x = lat;//fixme
	y = lon; //fixme
	start = false;
	vel[0] = v;
	vel[1] = 0;
	vel[2] = 0;
}

void Aircraft::rotate(double d_pitch, double azimuth)
{
	double tmp[3] = {0, 0, 1};

	rotation_matrix[0] = cos(M_PI / 180 * azimuth); rotation_matrix[1] = -sin(M_PI / 180 * azimuth); rotation_matrix[2] = 0;
	rotation_matrix[3] = sin(M_PI / 180 * azimuth); rotation_matrix[4] = cos(M_PI / 180 * azimuth); rotation_matrix[5] = 0;
	rotation_matrix[6] = 0;						    rotation_matrix[7] = 0; 						rotation_matrix[8] = 1;
	vec3_matrix33_mult(&vel, rotation_matrix);
	decline(&vel, tmp, d_pitch);
}

[[noreturn]] void Aircraft::fly()
{
	double u = 0; // управляющее воздействие на угол тангажа
	double dt = 1e-3;
	double time = 0;
	double feedback = 0; // обратная связь

	while (true)
	{
		//pitch = feedback; // сохраняем предыдущий фидбек

		u = pitch_control(time, 0) - feedback;
		//feedback = rungeKutta(time, time + dt, feedback, 1e-5, pitch_control); // получаем новый
		feedback = feedback + u * dt; // получаем новый

		std::cout << feedback << std::endl;
//		pitch = (feedback - pitch) / dt; // вычисление текущего угла курса
//		u -= feedback; // вычисление управления с учетом обратной связи

		std::this_thread::sleep_for(1ms);
		time += dt;
	}
}

const double *Aircraft::get_state() const
{
	return this->state;
}

Aircraft::~Aircraft()
{
	delete []vel;
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