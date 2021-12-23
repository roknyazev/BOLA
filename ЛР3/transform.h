//
// Created by Князев Роман Олегович on 17.12.2021.
//

#ifndef LW1_TRANSFORM_H
#define LW1_TRANSFORM_H

#include <cmath>
#include "ARINC429.h"
#include <map>

uint32_t	direct_transform(int bits, uint32_t address, float value);
float 		reverse_transform(int bits, ARINC value);

std::map<uint32_t, float> table();


#endif //LW1_TRANSFORM_H
