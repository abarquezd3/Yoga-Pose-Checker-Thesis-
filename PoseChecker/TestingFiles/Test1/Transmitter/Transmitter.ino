#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const uint64_t address = 250;

RF24 radio(9,10); //CE, CSN
float values[8] = {0.025, -87.5988, -22.05889, 44.646, 798.45321, -845.123, 98.651, 0.54};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  //radio.startListening();
  radio.stopListening();
}

void loop() {
  //Write the data  
  Serial.print(radio.write(&values, sizeof(values)));
  delay(600);
}
