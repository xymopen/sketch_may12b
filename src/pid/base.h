#pragma once

#ifndef _BASE_PID_H
#define _BASE_PID_H

template <class T>
class _base_pid
{
  public:
	typedef T value_type;
	T kp, ki, kd;

  protected:
	_base_pid(T kp = 1, T ki = 0, T kd = 0);
};

#include "base.impl.hpp"

#endif // _BASE_PID_H
