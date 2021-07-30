#include "doublecheck.h"

bool doublecheck(unsigned char pin, decltype(millis()) duration) {
  auto start = millis();
  auto count = 0;
  auto total = 0;

  while (true) {
    auto now = millis();

    if (now - start > duration) {
      break;
    }

    total += 1;

    if (digitalRead(pin)) {
      count += 1;
    }
  }

  return count > (total / 2);
};
