#include <Arduino.h>

class decrementer {
 private:
  unsigned long count;
  bool infinite;

 public:
  decrementer(const unsigned long count);
  decrementer(const bool infinite);
  decrementer();
  void set(const unsigned long count);
  void set(const bool infinite);
  void clear();
  void add(const unsigned long count);
  bool operator()();
  bool is_positive();
  void decrement();
};
