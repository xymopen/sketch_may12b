#pragma once

#ifndef _PULSE_H
#define _PULSE_H

#include "src/kernel-list/kernel-list.h"
#include "Overflow.h"
#include "PinToggler.h"
#include "decrementer.h"

class Pulse {
 private:
  static unsigned long global_frequency;
  static kernel_list list;

  Overflow<unsigned long> overflow;
  PinToggler toggle;
  decrementer decrement;
  kernel_list_node node;

 public:
  static void updateGlobalfrequency(unsigned long frequency);
  static void isr();

  Pulse(unsigned char pin);

  void pulsing(bool infinite);
  void pulses(unsigned long count);
  void set_frequency(unsigned long frequency);
  bool done();
  void operator()();
};

#endif  // _PULSE_H
