#include "Receiver.h"

uint64_t address = 250;
Receiver receiver(address);

void setup() {
  Serial.begin(9600);
  receiver.begin();
}

void loop() {
  if(receiver.isAvailable())
  {
    receiver.receive();
    Serial.println(receiver.getSerialString());
  } else {
    Serial.println("Radio Unavailable");
  }
}
