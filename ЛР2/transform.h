//
// Created by Князев Роман Олегович on 17.12.2021.
//

#ifndef LW1_TRANSFORM_H
#define LW1_TRANSFORM_H

#include <cmath>
#include "ARINC429.h"

#define RemoveBits(x, p) (float_t) (((float_t) ((x) * ((p)+1))) - (float_t) (((float_t) ((x) * ((p)+1))) - (x)))

//int*		massbin(double max_value, int max_digit, int digit, double value);
//int			bindec(double max_value, int max_digit, int digit, double value);
//double*		decbin(int dec, int max_digit);
//int			massdec(double max_value, int max_digit, int digit, int dec);
uint32_t	direct_transform(int bits, uint32_t address, float value);
float 		reverse_transform(int bits, ARINC value);

union conversion
{
	uint32_t	raw;
	float_t		float_nmb;
};

#endif //LW1_TRANSFORM_H
