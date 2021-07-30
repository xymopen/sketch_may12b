#pragma once

#ifndef _BASE_PID_IMPL_H
#define _BASE_PID_IMPL_H

template <class T>
_base_pid<T>::_base_pid(T kp, T ki, T kd) : kp(kp), ki(ki), kd(kd)
{
}

#endif // _BASE_PID_IMPL_H
