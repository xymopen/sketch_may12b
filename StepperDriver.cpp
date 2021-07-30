#include "StepperDriver.h"

StepperDriver::StepperDriver(const unsigned char clk, const unsigned char dir)
    : clk(clk), dir(dir) {
  pinMode(dir, OUTPUT);

  digitalWrite(dir, HIGH);
};

void StepperDriver::setSpeed(unsigned long speed) {
  clk.set_frequency(speed / 2);
};

void StepperDriver::cw() {
  digitalWrite(dir, HIGH);
  clk.pulsing(true);
};

void StepperDriver::ccw() {
  digitalWrite(dir, LOW);
  clk.pulsing(true);
};

void StepperDriver::step(signed long steps) {
  bool cw = steps >= 0;

  digitalWrite(dir, cw);
  clk.pulses(cw ? steps : -steps);
}

void StepperDriver::speed(signed long speed) {
  bool cw = speed >= 0;

  digitalWrite(dir, cw);
  clk.pulsing(true);


  clk.set_frequency((cw ? speed : -speed) / 2);
}

bool StepperDriver::idle() { return clk.done(); };
