#pragma once

#ifndef _VELOCITY_PID_H
#define _VELOCITY_PID_H

#include "base.h"

template <class T>
class velocity_pid : public _base_pid<T>
{
public:
	velocity_pid(T kp = 0, T ki = 1, T kd = 0);
	T operator()(T input, T target);

protected:
	T last, last2;
};

#include "velocity.impl.hpp"

#endif // _VELOCITY_PID_H
