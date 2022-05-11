#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <Arduino.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

class Transmitter
{
  public:
    Transmitter(uint64_t address);
    void transmit(float data[8]);
    void begin();
  private:
    uint64_t address;
    RF24 radio;
};

#endif
