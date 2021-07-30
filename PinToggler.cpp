#include "PinToggler.h"
#include <Arduino.h>

PinWriter::PinWriter(const unsigned char pin)
    : bsrr(&((PIN_MAP[pin].gpio_device)->regs->BSRR)),
      smask(BIT(PIN_MAP[pin].gpio_bit)),
      rmask(smask << 16) {
  pinMode(pin, OUTPUT);
}

void PinWriter::operator()(bool level) {
  if (level) {
    *bsrr = smask;
  } else {
    *bsrr = rmask;
  }
}

PinToggler::PinToggler(const unsigned char pin) : PinToggler(pin, LOW) {}

PinToggler::PinToggler(const unsigned char pin, const bool level)
    : write(pin),
      level(level) {
  pinMode(pin, OUTPUT);
  operator()();
}

void PinToggler::operator()() {
  write(level);
  level = !level;
}
