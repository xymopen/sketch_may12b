#include "Pulse.h"

unsigned long Pulse::global_frequency = 100000;
kernel_list Pulse::list;

void Pulse::updateGlobalfrequency(unsigned long frequency) {
  global_frequency = frequency;
}

void Pulse::isr() {
  for (auto &pulse : kernel_list_iterable<Pulse>(list, &Pulse::node)) {
    pulse();
  }
}

Pulse::Pulse(unsigned char pin) : overflow(), toggle(pin), decrement(), node() {
  list.push_back(node);
  pulses(false);
}

void Pulse::pulsing(bool infinite) { decrement.set(infinite); }

void Pulse::pulses(unsigned long count) { decrement.add(count); }

void Pulse::set_frequency(unsigned long frequency) {
  if (0 == frequency) {
    decrement.set((bool)false);
  } else {
    // frequency *= 2;

    // overflow.set(global_frequency / gcd(frequency, global_frequency),
    //             global_frequency / frequency);

    overflow.set(global_frequency / frequency,
                global_frequency / frequency / 2);
  }

}

bool Pulse::done() { return !decrement.is_positive(); }

void Pulse::operator()() {
  if (overflow() && decrement()) {
    toggle();
  }
}
