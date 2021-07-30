#pragma once

#ifndef _PERSISIT_TARGET_H
#define _PERSISIT_TARGET_H

// Mixin for PID to persist the target
template <class pid_t>
class persist_target : public pid_t
{
public:
	using value_type = typename pid_t::value_type;

	template <class... T>
	persist_target(value_type target, T... t);

	void set(value_type target);
	value_type operator()(value_type input);
	value_type operator()(value_type input, value_type target);

protected:
	value_type target;
};

#include "persist-target.impl.hpp"

#endif // _PERSISIT_TARGET_H
