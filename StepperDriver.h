#ifndef _STEPPER_DRIVER_H
#define _STEPPER_DRIVER_H

#include <Arduino.h>
#include "Pulse.h"

class StepperDriver {
 private:
  Pulse clk;
  const unsigned char dir;

 public:
  StepperDriver(const unsigned char clk, const unsigned char dir);

  void setSpeed(unsigned long speed);
  void cw();
  void ccw();
  void step(signed long steps);
  void speed(signed long speed);
  bool idle();
};

#endif  // _STEPPER_DRIVER_H
