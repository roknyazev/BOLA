//
// Created by Князев Роман Олегович on 23.12.2021.
//

#include "Aircraft.h"

Aircraft::Aircraft()
{
}

Aircraft::Aircraft(double lat, double lon, double alt, double az, double v0, std::list<double *> *checkpoints):
lat_lon(),
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
rotation_matrix(),
checkpoints(checkpoints),
xyz()
{

	//fixme добавить мютекс
	lat_lon[0] = lat;
	lat_lon[1] = lon;
	init_lat_lon[0] = lat;
	init_lat_lon[1] = lon;

	vel = new double[3];



	start = false;
	vel[0] = v;
	vel[1] = 0;
	vel[2] = 0;

	rotate(0, az);

}

void Aircraft::rotate(double d_pitch, double d_azimuth)
{
	double tmp[3] = {0, 0, 1};

	rotation_matrix[0] = cos(M_PI / 180 * d_azimuth); rotation_matrix[1] = -sin(M_PI / 180 * d_azimuth); rotation_matrix[2] = 0;
	rotation_matrix[3] = sin(M_PI / 180 * d_azimuth); rotation_matrix[4] = cos(M_PI / 180 * d_azimuth); rotation_matrix[5] = 0;
	rotation_matrix[6] = 0;						    rotation_matrix[7] = 0; 						rotation_matrix[8] = 1;


	auto vec = vec3_matrix33_mult(vel, rotation_matrix);
	vel[0] = vec[0];
	vel[1] = vec[1];
	vel[2] = vec[2];

	vec = decline(vel, tmp, d_pitch);
	vel[0] = vec[0];
	vel[1] = vec[1];
	vel[2] = vec[2];
}

void Aircraft::fly()
{
	double u;
	double dt = 1e-3;
	double time = 0;
	double pitch_feedback = 0;// обратная связь
	double roll_feedback = 0;// обратная связь
	auto curr_checkpoint_it = checkpoints->begin();
	double target_az;
	double prev_pitch;
	std::vector<double> geogr(3);
	std::vector<double> st(3);
	double dist;
	double prev_az;
	FILE *res = fopen("../../../sfm/res.csv", "w");

	fprintf(res, "x,y,z,pitch,az\n");
	while (true)
	{
		prev_pitch = pitch;
		pitch = pitch_feedback; // сохраняем предыдущий фидбек

		u = pitch_control(time, 0) - pitch_feedback; // расчет функции с учетом обратной связи
		//pitch_feedback = rungeKutta(time, time + dt, pitch_feedback, 1e-5, pitch_control); // получаем новый
		pitch_feedback = pitch_feedback + u * dt; // получаем новый
		pitch = (pitch_feedback - pitch) / dt * 10; // вычисление текущего угла курса

		prev_az = az;
		target_az = get_az(lat_lon, *curr_checkpoint_it);
		roll_feedback = roll_feedback + (target_az - roll_feedback) * dt;
		az = roll_feedback;
		//std::cout << az << std::endl;
		rotate(pitch - prev_pitch, az - prev_az);

		xyz[0] = xyz[0] + vel[0] * dt;
		xyz[1] = xyz[1] + vel[1] * dt; // построить крен
		xyz[2] = xyz[2] + vel[2] * dt;
		fprintf(res, "%f,%f,%f,%f,%f\n", xyz[0], xyz[1], xyz[2], pitch, az);
		//std::cout << xyz[0] << ',' << xyz[1] << ',' << xyz[2] << std::endl;
		st[0] = xyz[0];
		st[1] = xyz[1];
		st[2] = xyz[2];

		geogr = fromStart2Geogr(st, M_PI / 180 * init_lat_lon[1], M_PI / 180 * init_lat_lon[0], table);
		lat_lon[1] = geogr[0] / M_PI * 180;
		lat_lon[0] = geogr[1] / M_PI * 180;
		alt = geogr[2];

		dist = calcDist(lat_lon, *curr_checkpoint_it);
		std::cout << dist << std::endl;

		if (dist < 1 && curr_checkpoint_it == checkpoints->end())
		{
			return;
		}
		if (dist < 1)
		{
			curr_checkpoint_it++;
		}

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