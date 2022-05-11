#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

RF24 radio(9, 10);  //CE, CSN

void setup() {
  Serial.begin(9600);
  //Initialize
  if(!radio.begin())
  {
    Serial.println("Radio is not Responding");
  } else 
  {
    Serial.println("Radio communication successful");
  }
}

void loop() {
}
