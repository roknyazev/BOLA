//
// Created by OUT-Knyazev-RO on 27.09.2021.
//
#include <chrono>
#include <thread>

#ifndef LW1_TIMER_H
#define LW1_TIMER_H

using namespace std::chrono;

template <typename Arg>
ssize_t timer(ssize_t (*callback)(Arg), Arg arg, milliseconds duration)
{
	auto start = steady_clock::now();
	int error;

	error = callback(arg);
	while (steady_clock::now() < start + duration)
		std::this_thread::sleep_for(1ms);
	return error;
}

#endif //LW1_TIMER_H
