#pragma once

#ifndef _POSITION_PID_IMPL_H
#define _POSITION_PID_IMPL_H

template <class T>
position_pid<T>::position_pid(T kp, T ki, T kd) : _base_pid<T>(kp, ki, kd), last(0), integ(0)
{
}

template <class T>
T position_pid<T>::operator()(T input, T target)
{
	T output, error = input - target, diff = error - this->last;

	integ += error;
	output = this->kp * error + this->ki * integ + this->kd * diff;
	last = error;

	return output;
}

#endif // _POSITION_PID_IMPL_H
