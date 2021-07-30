#include "decrementer.h"

decrementer::decrementer(const unsigned long count)
    : count(count), infinite(false) {
  /* empty */;
}

decrementer::decrementer(const bool infinite) : count(0), infinite(infinite) {
  /* empty */;
}

decrementer::decrementer() : count(0), infinite(false) { /* empty */
  ;
}

void decrementer::set(const unsigned long count) {
  this->count = count;
  infinite = false;
}

void decrementer::set(const bool infinite) {
  count = 0;
  this->infinite = infinite;
}

void decrementer::clear() { set((bool)false); }

void decrementer::add(const unsigned long count) { set(this->count + count); }

bool decrementer::operator()() {
  if (infinite) {
    return true;
  } else if (count > 0) {
    count -= 1;

    return true;
  } else {
    return false;
  }
}

bool decrementer::is_positive() { return infinite || count > 0; }

void decrementer::decrement() {
  if (count > 0) {
    count -= 1;
  }
}
