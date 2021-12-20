//
// Created by Князев Роман Олегович on 20.12.2021.
//

#include "Sns.h"

Sns::Sns(std::mutex *m, milliseconds p, milliseconds tcd, milliseconds pd) :
		mutex(m),
		packed_state(),
		measure_period(p),
		test_control_duration(tcd),
		prepare_duration(pd)
{
}

void Sns::test_control()
{
	ARINC tmp = {};

	tmp.discrete.address = 0273;
	tmp.discrete.control = true;
	tmp.discrete.readiness = true;
	mutex->lock();
	packed_state[12].raw = tmp.raw;
	mutex->unlock();
}

void Sns::navigation()
{
	ARINC tmp1 = {};
	ARINC tmp2 = {};

	tmp1.bcd.address = 0260;
	tmp1.bcd.hours = 20; // год
	tmp1.bcd.minutes = 7; // месяц
	tmp1.bcd.seconds = 31; // день


	tmp2.discrete.address = 0273;
	tmp2.discrete.control = true;
	tmp2.discrete.readiness = true;
	tmp2.discrete.navigation = true;

	mutex->lock();
	packed_state[0].raw = direct_transform(20, 076, 12.567891);
	packed_state[1].raw = direct_transform(20, 0101, 12.5);
	packed_state[2].raw = direct_transform(20, 0102, 1048);
	packed_state[3].raw = direct_transform(20, 0103, 12);
	packed_state[4].raw = direct_transform(20, 0110, 15);
	packed_state[5].raw = direct_transform(20, 0120, 15);
	packed_state[6].raw = direct_transform(20, 0111, 170.4);
	packed_state[7].raw = direct_transform(20, 0121, 170.4);
	packed_state[8].raw = direct_transform(20, 0113, 382.88);
	packed_state[9].raw = direct_transform(20, 0150, 1.02);
	packed_state[10].raw = direct_transform(20, 0140, 1.1);
	packed_state[11].raw = tmp1.raw;
	packed_state[12].raw = tmp2.raw;
	mutex->unlock();
}


ssize_t sns_test_control(Sns &sns)
{
	sns.test_control();
	return 0;
}

ssize_t sns_test_control_timer(Sns &sns)
{
	timer<Sns &>(sns_test_control, sns, sns.measure_period);
	return 0;
}



ssize_t sns_navigation(Sns &sns)
{
	sns.navigation();
	return 0;
}

ssize_t sns_navigation_timer(Sns &sns)
{
	timer<Sns &>(sns_navigation, sns, sns.measure_period);
	return 0;
}

void sns_work(Sns &sns)
{
	ssize_t error = 0;
	cycle_timer<Sns &>(sns_test_control_timer, sns, sns.test_control_duration, -1);
	while (error != -1)
		error = sns_navigation_timer(sns);
}
