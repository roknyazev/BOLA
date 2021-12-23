//
// Created by Князев Роман Олегович on 23.12.2021.
//

#ifndef LW1_AIRCRAFT_H
#define LW1_AIRCRAFT_H
#include <random>
#include "../Timer.h"
#include "../transform.h"

class Aircraft
{
private:

	float x;
	float y;
	float v;

	float lat;
	float lon;
	float alt;
	float az; //курс истинный
	float pitch; //тангаж
	float roll; //крен
	float v_x; // скорость север
	float v_y; // скорость восток
	float v_z; // вертикальная скорость
	float a_x;
	float a_y;
	float a_z;
private:
	float state[12];

	[[noreturn]] void fly();
public:
	Aircraft(float lat, float lon, float alt, float az, float v0);
	Aircraft();
	bool start;
	const float *get_state() const;
};



#endif //LW1_AIRCRAFT_H
