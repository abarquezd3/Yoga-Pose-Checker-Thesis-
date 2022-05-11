#include "Reading.h"

Reading::Reading()
{
  Wire.begin();
}

void Reading::readChannels()
{
 Serial.println("Scanning Channels");

  //Loop through possible channels
  for(uint8_t t = 0; t < 8; t++)
  {
      switchChannel(t);
      Serial.print("TCA Port #"); Serial.println(t);

      //Search for all I2C devices connected to bus (1 I2C bus can hold 128 devices)
      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == MUX) continue; //Avoid conflict with MUX

        Wire.beginTransmission(addr); 

        if (!Wire.endTransmission()) { //If Success
          Serial.print("Found I2C 0x");  Serial.println(addr,HEX); //Print addr in HEX
        }
      }
  }
  Serial.println("\ndone");
}

void Reading::switchChannel(int channel)
{
  Wire.beginTransmission(MUX);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

float Reading::readAcc()
{
  //Accelerometer value is 10 bits but can only send 8 bits at a time, 
  //so 1st Wire.read() is last 2 bits, then 2nd Wire.read() is for the first 8 bits
  float value = Wire.read() << 8 | Wire.read();
  return value/16384; //To limit range to +- 2g
}

float* Reading::readValues(int ACCBUS)
{
  switchChannel(ACCBUS);
  //Communicate
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B); //Access beginning value register
  Wire.endTransmission(false); //End connection while retaining access
  Wire.requestFrom(MPU6050, 6, true); //Read 6 registers from 0x3B
  values[1] = readAcc() + calibrations[ACCBUS][1];
  values[0] = readAcc() + calibrations[ACCBUS][0];
  values[2] = readAcc() + calibrations[ACCBUS][2];

  values[3] = atan(values[1] / sqrt( pow(values[0], 2) + pow(values[2], 2))) * 180/PI; //Roll
  values[4] = atan(-1 * values[2] / sqrt( pow(values[1], 2) + pow(values[0], 2))) * 180/PI; //Pitch
  return values;
}
