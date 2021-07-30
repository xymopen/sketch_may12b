#include <stdint.h>

class PinWriter {
 private:
  volatile uint32_t* const bsrr;  // BSRR set register (lower 16 bits)
  const uint32_t smask;           // BSRR set bitmask
  const uint32_t rmask;           // BSRR reset bitmask
 public:
  PinWriter(const unsigned char pin);
  void operator()(bool level);
};

class PinToggler {
 private:
  PinWriter write;
  bool level;

 public:
  PinToggler(const unsigned char pin);
  PinToggler(const unsigned char pin, const bool level);
  void operator()();
};
