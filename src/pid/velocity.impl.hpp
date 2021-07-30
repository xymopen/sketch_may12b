#pragma once

#ifndef _VELOCITY_PID_IMPL_H
#define _VELOCITY_PID_IMPL_H

template <class T>
velocity_pid<T>::velocity_pid(T kp, T ki, T kd) : _base_pid<T>(kp, ki, kd), last(0), last2(0)
{
}

template <class T>
T velocity_pid<T>::operator()(T input, T target)
{
	T output, error = input - target, diff = error - last;

	output = this->kp * diff + this->ki * error + this->kd * (diff - last + last2);
	last = error;
	last2 = last;

	return output;
}

#endif // _VELOCITY_PID_IMPL_H
