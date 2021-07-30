#pragma once

#ifndef _OVERFLOW_IMPL_H
#define _OVERFLOW_IMPL_H

#include "src/gcd/gcd.h"

template <class count_t>
Overflow<count_t>::Overflow() : Overflow(1, 1){}

template <class count_t>
Overflow<count_t>::Overflow(count_t prescale, count_t overflow)
    : count(0), prescale(prescale), overflow(overflow){}

template <class count_t>
void Overflow<count_t>::set(count_t prescale, count_t overflow) {
  this->prescale = prescale;
  this->overflow = overflow;

  if (count >= overflow) {
    count = overflow - 1;
  }
}

template <class count_t>
void Overflow<count_t>::reset() {
  count = 0;
}

template <class count_t>
bool Overflow<count_t>::operator()() {
  count += 1;

  if (count == prescale) {
    count = 0;

    return true;
  } else if (count == overflow) {
    return true;
  } else {
    return false;
  }
}

#endif  // _OVERFLOW_IMPL_H
