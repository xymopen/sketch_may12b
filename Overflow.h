#pragma once

#ifndef _OVERFLOW_H
#define _OVERFLOW_H

template <class count_t>
class Overflow {
 private:
  count_t count;
  count_t prescale;
  count_t overflow;

 public:
  Overflow();
  Overflow(count_t prescale, count_t overflow);
  void set(count_t prescale, count_t overflow);
  void reset();
  bool operator()();
};

#include "Overflow.impl.hpp"

#endif	// _OVERFLOW_H
