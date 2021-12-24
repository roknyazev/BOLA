//
// Created by Князев Роман Олегович on 17.12.2021.
//

#include "Ins.h"

Ins::Ins(std::mutex *m, milliseconds p, milliseconds tcd, milliseconds pd, Aircraft *ac) :
mutex(m),
packed_state(),
measure_period(p),
test_control_duration(tcd),
prepare_duration(pd),
aircraft(ac)
{
}

void Ins::test_control()
{
	ARINC tmp = {};

	tmp.discrete.address = 184;
	tmp.discrete.control = true;
	tmp.discrete.INS_serviceability = true;
	tmp.discrete.no_initial_data = true;
	mutex->lock();
	packed_state[12].raw = tmp.raw;
	mutex->unlock();

}

void Ins::prepare()
{
	ARINC tmp = {};

	tmp.discrete.address = 184;
	tmp.discrete.preparation = true;
	tmp.discrete.INS_serviceability = true;
	tmp.discrete.no_initial_data = false;
	mutex->lock();
	for (int i = 0; i < 12; i++)
		packed_state[i].bnr.SM = 3;
	packed_state[12].raw = tmp.raw;
	mutex->unlock();

}

void Ins::navigation()
{
	ARINC tmp = {};
	tmp.discrete.address = 184;
	tmp.discrete.preparation = true;
	tmp.discrete.INS_serviceability = true;
	tmp.discrete.no_initial_data = false;
	tmp.discrete.readiness = true;
	if (!aircraft->start)
	{
		aircraft->start = true;
	}


	mutex->lock();

	packed_state[0].raw = direct_transform(20, 0310, -122.513);
	packed_state[1].raw = direct_transform(20, 0311, 22.51);
	packed_state[2].raw = direct_transform(20, 0361, 3048);
	packed_state[3].raw = direct_transform(20, 0314, 10);
	packed_state[4].raw = direct_transform(20, 0324, 5);
	packed_state[5].raw = direct_transform(20, 0325, 45);
	packed_state[6].raw = direct_transform(20, 0366, 370);
	packed_state[7].raw = direct_transform(20, 0367, 370.4);
	packed_state[8].raw = direct_transform(20, 0365, 182.88);
	packed_state[9].raw = direct_transform(20, 0331, 0.02);
	packed_state[10].raw = direct_transform(20, 0332, 0.1);
	packed_state[11].raw = direct_transform(20, 0333, 0.131231232131221111);
	packed_state[12].raw = tmp.raw;
	mutex->unlock();
}

ssize_t ins_test_control(Ins &ins)
{
	ins.test_control();
	return (0);
}

ssize_t ins_test_control_timer(Ins &ins)
{
	timer<Ins &>(ins_test_control, ins, ins.measure_period);
	return (0);
}

ssize_t ins_prepare(Ins &ins)
{
	ins.prepare();
	return (0);
}

ssize_t ins_prepare_timer(Ins &ins)
{
	timer<Ins &>(ins_prepare, ins, ins.measure_period);
	return (0);
}

ssize_t ins_navigation(Ins &ins)
{
	ins.navigation();
	return (0);
}

ssize_t ins_navigation_timer(Ins &ins)
{
	timer<Ins &>(ins_navigation, ins, ins.measure_period);
	return (0);
}

void ins_work(Ins &ins)
{
	ssize_t error = 0;
	cycle_timer<Ins &>(ins_test_control_timer, ins, ins.test_control_duration, -1);
	cycle_timer<Ins &>(ins_prepare_timer, ins, ins.prepare_duration, -1);
	while (error != -1)
		error = ins_navigation_timer(ins);
}
