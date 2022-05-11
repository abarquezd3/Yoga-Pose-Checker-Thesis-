#include <Wire.h>

const int MPU6050 = 0x68,
          MUX = 0x70,
          bus = 6;
float calR = 0.00, calP = 0.00;

const String base = "";

void setup() {
  Serial.begin(9600);

  Wire.begin();
  
  switchChannel(bus);
  //Reset Connections MPU6050
  Wire.beginTransmission(MPU6050);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  calculateErrors();
}

void loop() {
  //Request Data
  switchChannel(bus);
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 6, true);

  float x = readAcc();
  float y = readAcc();
  float z = readAcc();
          
  float roll = (atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI) + calR; 
  float pitch = (atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI) + calP; 

  Serial.println(base + (String)x + "," + y + "," + z + " : " + roll + "," + pitch);
}

//Tests from 200 samples
void calculateErrors()
{
  int c = 0;
  float accX, accY, accZ;
          
  while (c < 200) {
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 6, true);
  accX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
  accY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
  accZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
  // Sum all readings
  float accErrX, accErrY;
  accErrX = accErrX + ((atan((accY) / sqrt(pow((accX), 2) + pow((accZ), 2))) * 180 / PI));
  accErrY = accErrY + ((atan(-1 * (accX) / sqrt(pow((accY), 2) + pow((accZ), 2))) * 180 / PI));
  c++;
  }
  //Divide the sum by 200 to get the error value
  accErrX = accErrX / 200;
  accErrY = accErrY / 200;  
  Serial.println(base + "Errors: X: " + (String)accErrX + " Y: " + accErrY);
}

void switchChannel(int channel)
{
  Wire.beginTransmission(MUX);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

float readAcc()
{
  float value = Wire.read() << 8 | Wire.read();
  return value/16384; //+-2g
}
