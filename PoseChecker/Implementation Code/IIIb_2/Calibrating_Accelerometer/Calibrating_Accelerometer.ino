#include <Wire.h>

#define xOffset 0.00
#define yOffset 0.00
#define zOffset 0.00

const int MPU6050 = 0x68,
          MUX = 0x70,
          ACCBUS = 7;

const String base = "";

void setup() {
  Wire.begin();
  
  //Set acc to reading mode
  switchChannel(ACCBUS);
  //Set ADXL345 to Measuring Mode
  Wire.beginTransmission(MPU6050);
  Wire.write(0x6B); //Access Power Register
  Wire.write(0x00); //Write for Measuring mode
  Wire.endTransmission(true);
  delay(10);
  
  Serial.begin(9600);
}

void loop() {
  //Read values from 6 & 7
  String value = "";

  switchChannel(ACCBUS);
  //Communicate
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B); //Access beginning value register
  Wire.endTransmission(false); //End connection while retaining access
  Wire.requestFrom(MPU6050, 6, true); //Read 6 registers from 0x3B
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
  //so 1st Wire.read() is last 2 bits, then 2nd Wire.read() is for the first 8 bits
  float value = Wire.read() << 8 | Wire.read();
  return value/16384; //To limit range to +- 2g
}

//Tests from 200 samples
void calculateErrors()
{
  int c = 0;
  float accX, accY, accZ;
  float accErrX, accErrY;
  while (c < 200) {
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 6, true);
  accX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
  // Sum all readings
  accErrX = accErrX + ((atan((accY) / sqrt(pow((accX), 2) + pow((accZ), 2))) * 180 / PI));
  accErrY = accErrY + ((atan(-1 * (accX) / sqrt(pow((accY), 2) + pow((accZ), 2))) * 180 / PI));
  c++;
  }
  //Divide the sum by 200 to get the error value
  accErrX = accErrX / 200;
  accErrY = accErrY / 200;  
  Serial.println(base + "Errors: X: " + (String)accErrX + " Y: " + accErrY);
}
