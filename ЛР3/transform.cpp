//
// Created by Князев Роман Олегович on 20.12.2021.
//
#include "transform.h"
#include <bitset>

std::map<uint32_t , float> table()
{
	std::map<uint32_t , float> result;

	result[0310] = 90;
	result[0311] = 90;
	result[0361] = 19975.3728;
	result[0314] = 90;
	result[0324] = 90;
	result[0325] = 90;
	result[0366] = 3792.896;
	result[0367] = 3792.896;
	result[0365] = 4993.8432;
	result[0331] = 39.24;
	result[0332] = 39.24;
	result[0333] = 39.24;

	result[076] = 65536;
	result[0101] = 512;
	result[0102] = 512;
	result[0103] = 90;
	result[0110] = 90;
	result[0120] = 0.0000858;
	result[0111] = 90;
	result[0121] = 0.0000858;
	result[0113] = 512;
	result[0150] = 16;
	result[0140] = 512;
	result[0165] = 16384;
	return result;
}

std::map<uint32_t , float> TABLE = table();

uint32_t direct_transform(int bits, uint32_t address, float value)
{
	ARINC res = {};
	uint32_t nmb;
	char str_nmb[21] = "00000000000000000000";
	float high_bit_value = TABLE[address];

	if (value < 0)
	{
		res.bnr.sign = 1;
		value = -value;
	}
	for (int i = 0; i < bits; i++)
	{
		if (value > high_bit_value)
		{
			str_nmb[i] = '1';
			value -= (float)high_bit_value;
		}
		high_bit_value /= 2;
	}
	nmb = std::stoi(str_nmb, nullptr, 2);
	res.bnr.address = address;
	res.bnr.height = nmb;
	res.bnr.SM = 3;
	res.bnr.P = 1;
	return res.raw;
}

float reverse_transform(int bits, ARINC value)
{
	std::string str_nmb = std::bitset<32>(value.bnr.height).to_string().substr(12, 20);
	float result = 0;
	float high_bit_value = TABLE[value.bnr.address];

	for (int i = 0; i < bits; i++)
	{
		if (str_nmb[i] == '1')
			result += high_bit_value;
		high_bit_value /= 2;
	}
	if (value.bnr.sign)
		result = -result;
	return result;
}
