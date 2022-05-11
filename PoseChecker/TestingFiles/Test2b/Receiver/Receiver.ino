#include <Wire.h>

#define MUX 0x70

void setup() {
  while (!Serial); //Wait for Serial Port
  delay(1000);     //Give time for Serial Port to Stabilize

  Wire.begin();

  Serial.begin(9600);
  Serial.println("\nTCAScanner ready!");

  //Loop through possible channels
  for(uint8_t t = 0; t < 8; t++)
  {
      switchChannel(t);
      Serial.print("TCA Port #"); Serial.println(t);

      //Search for all I2C devices connected to bus (1 I2C bus can hold 128 devices)
      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == MUX) continue; //Avoid conflict with MUX

        Wire.beginTransmission(addr); 

        //endTransmission() returns bytes based on status of transmission
        // 0 - Success
        // 1 - Data bigger than transmit buffer
        // 2 - Received NACK when transmitting address (NACK - Not Acknowledged)
        // 3 - Received NACK when transmitting data
        if (!Wire.endTransmission()) { //If Success
          Serial.print("Found I2C 0x");  Serial.println(addr,HEX); //Print addr in HEX
        }
      }
  }
  Serial.println("\ndone");

}

//Switch Channel from 0-7
void switchChannel(uint8_t bus)
{
  if (bus > 7) return;
  //Write value in MUX in 8 bits that must have only 1 high bit
  //The position of the high bit is the channel connected by MUX
  //ex: 00100000 means channel 5
  Wire.beginTransmission(MUX);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void loop() { }
