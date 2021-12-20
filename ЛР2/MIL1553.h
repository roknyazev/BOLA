//
// Created by OUT-Knyazev-RO on 17.09.2021.
//

#ifndef LW1_MIL1553_H
#define LW1_MIL1553_H
#include <iostream>

#pragma pack(push, 1)
    struct CW
    {
        unsigned int sync_signal: 3;
        unsigned int address: 5;
        unsigned int K: 1;
        unsigned int control_mode: 5;
        unsigned int command_code: 5;
        unsigned int P: 1;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct IW
    {
        unsigned int sync_signal: 3;
        unsigned int data: 16;
        unsigned int P: 1;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct RW
    {
        unsigned int sync_signal: 3;
        unsigned int address: 5;
        unsigned int error_in_message: 1;
        unsigned int transmission: 1;
        unsigned int service_request: 1;
        unsigned int reserve: 3;
        unsigned int group_command_accepted: 1;
        unsigned int subscriber_is_busy: 1;
        unsigned int subscriber_malfunction: 1;
        unsigned int interface_control_accepted: 1;
        unsigned int malfunction: 1;
        unsigned int P: 1;
    };
#pragma pack(pop)

union MIL1553 {
	uint32_t raw;
	CW bnr;
	IW bcd;
	RW discrete;
};

#endif //LW1_MIL1553_H
