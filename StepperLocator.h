#ifndef _STEPPER_LOCATION_H
#define _STEPPER_LOCATION_H

#include "StepperDriver.h"

class StepperLocator : public StepperDriver {
 private:
  signed long loc;

 public:
  StepperLocator(const unsigned char clk, const unsigned char dir,
                           const signed long loc = 0);

  void updateLocation(const signed long location);
  signed long getLocation();
  void locate(const signed long location);
};

StepperLocator::StepperLocator(const unsigned char clk, const unsigned char dir,
                         const signed long loc)
    : StepperDriver(clk, dir), loc(loc){};

inline void StepperLocator::updateLocation(const signed long location) {
  loc = location;
};

inline signed long StepperLocator::getLocation() { return loc; };

inline void StepperLocator::locate(const signed long location) {
  StepperDriver::step(location - loc);
  loc = location;
};

#endif  // _STEPPER_LOCATION_H
