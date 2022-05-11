#include "Transmitter.h"

//address
const uint64_t address = 250;
float values[8] = {0.025, -87.5988, -22.05889, 44.646, 798.45321, -845.123, 98.651, 0.54};

Transmitter transmitter(address);

void setup() {
  transmitter.begin();
}

void loop() {
  transmitter.transmit(values);
  delay(300);
}
