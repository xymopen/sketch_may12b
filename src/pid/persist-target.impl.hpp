#pragma once

#ifndef _PERSISIT_TARGET_IMPL_H
#define _PERSISIT_TARGET_IMPL_H

template <class pid_t>
template <class... T>
persist_target<pid_t>::persist_target(value_type target, T... t) : pid_t(t...), target(target)
{
}

template <class pid_t>
void persist_target<pid_t>::set(value_type target)
{
	this->target = target;
}

template <class pid_t>
typename pid_t::value_type persist_target<pid_t>::operator()(value_type input)
{
	return pid_t::operator()(input, target);
}

template <class pid_t>
typename pid_t::value_type persist_target<pid_t>::operator()(value_type input,
															 value_type target)
{
	set(target);

	return operator()(input);
}

#endif // _PERSISIT_TARGET_IMPL_H
