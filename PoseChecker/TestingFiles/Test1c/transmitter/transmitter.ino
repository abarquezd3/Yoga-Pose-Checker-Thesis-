#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const uint64_t address = 250;

RF24 radio(9,10); //CE, CSN

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  //Write the data
  char message[] = "Hello World";
  radio.write(&message, sizeof(message));
  delay(600);
}
