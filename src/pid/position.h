#pragma once

#ifndef _POSITION_PID_H
#define _POSITION_PID_H

#include "base.h"

template <class T>
class position_pid : public _base_pid<T>
{
public:
	position_pid(T kp = 1, T ki = 0, T kd = 0);
	T operator()(T input, T target);

protected:
	T last, integ;
};

#include "position.impl.hpp"

#endif // _POSITION_PID_H
