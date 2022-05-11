#include "Reader.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

String base = "";
const int MPU6050 = 0x68,
          MUX = 0x70;
const uint64_t address = 250;
          
RF24 radio(9, 10); //CE, CSN
Reader reader;

//Float array (2 from Roll & pitch) * 4 Accelerometers = 7 values
float values[8];

void setup() {
  //Setup Wireless Transceiver
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();

  reader.begin();
}

void loop()
{
  reader.readValues(values);

  radio.write(&values, sizeof(values)); //Send data
  delay(100);
}
