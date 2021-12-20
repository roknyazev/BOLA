//
// Created by Князев Роман Олегович on 20.12.2021.
//

#ifndef LW1_SNS_H
#define LW1_SNS_H
#include <random>
#include "../Timer.h"
#include "../transform.h"

class Sns
{
public:
	ARINC packed_state[13];
	std::mutex *mutex;
	milliseconds measure_period;
	milliseconds test_control_duration;
	milliseconds prepare_duration;

	Sns(std::mutex *m, milliseconds p, milliseconds tcd, milliseconds pd);
	~Sns() = default;

	void test_control();
	void prepare();
	void navigation();
};

ssize_t sns_test_control(Sns &sns);
ssize_t sns_test_control_timer(Sns &sns);
ssize_t sns_prepare(Sns &sns);
ssize_t sns_prepare_timer(Sns &sns);
ssize_t sns_navigation(Sns &sns);
ssize_t sns_navigation_timer(Sns &sns);
void	sns_work(Sns &sns);

#endif //LW1_SNS_H
