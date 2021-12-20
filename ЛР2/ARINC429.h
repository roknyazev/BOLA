//
// Created by OUT-Knyazev-RO on 17.09.2021.
//

#ifndef LW1_ARINC429_H
#define LW1_ARINC429_H
#include <iostream>


#pragma pack(push, 1)
    struct BNR
    {
        uint32_t address: 8;
        uint32_t height: 20;
        uint32_t sign: 1;
        uint32_t SM: 2;
        uint32_t P: 1;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct BCD
	{
        uint32_t address: 8;
        uint32_t empty1: 3;
        uint32_t seconds: 6;
        uint32_t minutes: 6;
        uint32_t hours: 5;
        uint32_t empty2: 1;
        uint32_t SM: 2;
        uint32_t P: 1;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct Discrete
    {
        uint32_t address: 8;
        uint32_t SDI: 2;
        uint32_t preparation: 1;
        uint32_t control: 1;
        uint32_t navigation: 1;
        uint32_t gyrocompassing: 1;
        uint32_t empty1: 1;
        uint32_t re_launch: 1;
        uint32_t preparation_scale: 3;
        uint32_t heating_serviceability: 1;
        uint32_t thermostatting: 1;
        uint32_t no_initial_data: 1;
        uint32_t no_reception: 1;
        uint32_t INS_serviceability: 1;
        uint32_t readiness_accelerated: 1;
        uint32_t readiness: 1;
        uint32_t empty2: 3;
        uint32_t SM: 2;
        uint32_t P: 1;
    };
#pragma pack(pop)

union ARINC
{
	uint32_t raw;
	BNR bnr;
	BCD bcd;
	Discrete discrete;
};

#endif //LW1_ARINC429_H
