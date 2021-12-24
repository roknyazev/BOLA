//
// Created by Князев Роман Олегович on 23.12.2021.
//

#ifndef LW1_AIRCRAFT_H
#define LW1_AIRCRAFT_H
#include <random>
#include "../Timer.h"
#include "../transform.h"
#include "../mth.h"
#include "list"

class Aircraft
{
private:

	double xyz[3];

	double v;

	double lat_lon[2];
	double init_lat_lon[2];
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
	std::list<double *> *checkpoints;

public:
	Aircraft(double lat,
			 double lon,
			 double alt,
			 double az,
			 double v0,
			 std::list<double *> *checkpoints);
	~Aircraft();
	Aircraft();
	void fly();
	bool start;
	const double *get_state() const;
	void rotate(double d_pitch, double azimuth);

	std::map <int, double > table = { { 0, 111.3},
									  { 1, 111.3},
									  { 2, 111.3},
									  { 3, 111.2},
									  { 4, 111.1},
									  { 5, 110.9},
									  { 6, 110.7},
									  { 7, 110.5},
									  { 8, 110.2},
									  { 9, 110.0},
									  {10, 109.6},
									  {11, 109.3},
									  {12, 108.9},
									  {13, 108.5},
									  {14, 108.0},
									  {15, 107.6},
									  {16, 107.0},
									  {17, 106.5},
									  {18, 105.9},
									  {19, 105.3},
									  {20, 104.6},
									  {21, 104.0},
									  {22, 103.3},
									  {23, 102.5},
									  {24, 101.8},
									  {25, 101.0},
									  {26, 100.1},
									  {27,  99.3},
									  {28,  98.4},
									  {29,  97.4},
									  {30,  96.5},
									  {31,  95.5},
									  {32,  94.5},
									  {33,  93.5},
									  {34,  92.4},
									  {35,  91.3},
									  {36,  90.2},
									  {37,  89.0},
									  {38,  87.8},
									  {39,  86.6},
									  {40,  85.4},
									  {41,  84.1},
									  {42,  82.9},
									  {43,  81.5},
									  {44,  80.2},
									  {45,  78.8},
									  {46,  77.5},
									  {47,  76.1},
									  {48,  74.6},
									  {49,  73.2},
									  {50,  71.7},
									  {51,  70.2},
									  {52,  68.7},
									  {53,  67.1},
									  {54,  65.6},
									  {55,  64.0},
									  {56,  62.4},
									  {57,  60.8},
									  {58,  59.1},
									  {59,  57.5},
									  {60,  55.8},
									  {61,  54.1},
									  {62,  52.4},
									  {63,  50.7},
									  {64,  48.9},
									  {65,  47.2},
									  {66,  45.4},
									  {67,  43.6},
									  {68,  41.8},
									  {69,  40.0},
									  {70,  38.2},
									  {71,  36.4},
									  {72,  34.5},
									  {73,  32.6},
									  {74,  30.8},
									  {75,  28.9},
									  {76,  27.0},
									  {77,  25.1},
									  {78,  23.2},
									  {79,  21.3},
									  {80,  19.4},
									  {81,  17.5},
									  {82,  15.5},
									  {83,  13.6},
									  {84,  11.7},
									  {85,   9.7},
									  {86,   7.8},
									  {87,   5.8},
									  {88,   3.9},
									  {89,   1.9},
									  {90,     0} };
};

double pitch_control(double t, double y);
void start_aircraft(Aircraft &aircraft);

#endif //LW1_AIRCRAFT_H
