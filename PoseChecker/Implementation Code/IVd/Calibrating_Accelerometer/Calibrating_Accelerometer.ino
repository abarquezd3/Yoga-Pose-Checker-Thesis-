#include <Wire.h>

#define xOffset 0.00
#define yOffset 0.00
#define zOffset 0.00

const int ADXL345 = 0x53,
          MUX = 0x70,
          ACCBUS = 6;

const String base = "";

void setup() {
  Wire.begin();
  
  //Set acc to reading mode
  switchChannel(ACCBUS);
  //Set ADXL345 to Measuring Mode
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D); //Access Power Register
  Wire.write(8); //Write for Measuring mode
  Wire.endTransmission();
  delay(10);
  
  Serial.begin(9600);
}

void loop() {
  //Read values from 6 & 7
  String value = "";

  switchChannel(ACCBUS);
  //Communicate
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); //Access value register
  Wire.endTransmission(false); //End connection while retaining access
  Wire.requestFrom(ADXL345, 6, true);
  float x = readAcc() + xOffset;
  float y = readAcc() + yOffset;
  float z = readAcc() + zOffset;

  value += (String)x + "," + y + "," + z;

  Serial.println(value + "\n");
  delay(100);
}


void switchChannel(uint8_t bus)
{
  Wire.beginTransmission(MUX);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

float readAcc()
{
  //Accelerometer value is 10 bits but can only send 8 bits at a time, 
  //so 1st Wire.read() is 1st 8 bits, then 2nd Wire.read() is for the last 2 bits
  float value = Wire.read() | Wire.read() << 8;
  return value/256; //To limit range to +- 2g
}
