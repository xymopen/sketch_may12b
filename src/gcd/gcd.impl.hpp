#pragma once

#ifndef _GCD_IMPL_H
#define _GCD_IMPL_H

template <class T>
T gcd(T a, T b) {
  while (0 != b) {
    T t = b;
    b = a % b;
    a = t;
  }

  return a;
}

#endif	// _GCD_IMPL_H
