//
// Created by Князев Роман Олегович on 23.12.2021.
//

#include "Aircraft.h"

Aircraft::Aircraft()
{
}

Aircraft::Aircraft(float lat, float lon, float alt, float az, float v0):
lat(lat),
lon(lon),
alt(alt),

az(az),
v(v0),
pitch(0),
roll(0),

v_x(0),
v_y(0),
v_z(0),

a_x(0),
a_y(0),
a_z(0),

state(),
start(false)
{
	x = lat;//fixme
	y = lon; //fixme
	while (!start)
		std::this_thread::sleep_for(1ms);
	v_x = v;
	fly();
}

[[noreturn]] void Aircraft::fly()
{
	while (true)
	{

	}
}

const float *Aircraft::get_state() const
{
	return this->state;
}

