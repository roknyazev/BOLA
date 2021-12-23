//
// Created by Князев Роман Олегович on 17.12.2021.
//

#ifndef LW1_INS_H
#define LW1_INS_H


#include "../aircraft/Aircraft.h"

class Ins
{
public:
	ARINC packed_state[13];
	std::mutex *mutex;
	milliseconds measure_period;
	milliseconds test_control_duration;
	milliseconds prepare_duration;
	Aircraft *aircraft;

	Ins(std::mutex *m, milliseconds p, milliseconds tcd, milliseconds pd, Aircraft *ac);
	~Ins() = default;

	void test_control();
	void prepare();
	void navigation();
};

ssize_t ins_test_control(Ins &ins);
ssize_t ins_test_control_timer(Ins &ins);
ssize_t ins_prepare(Ins &ins);
ssize_t ins_prepare_timer(Ins &ins);
ssize_t ins_navigation(Ins &ins);
ssize_t ins_navigation_timer(Ins &ins);
void	ins_work(Ins &ins);



#endif //LW1_INS_H
