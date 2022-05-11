#include "Reader.h"

Reader::Reader()
{
}

void Reader::begin()
{
  //Setup I2C connection
  Wire.begin();
  for(int i = 0; i < 4; i++)
  {
    //Move to channel 2 (Accelerometer location)
    switchChannel(buses[i]);
    //Reset Connections MPU6050
    Wire.beginTransmission(MPU6050);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);
    delay(10);
  }
}

void Reader::readValues(float values[8])
{
  for(int i = 0; i < 4; i++)
  {
    //Switch to accelerometer
    switchChannel(buses[i]);
    //Communicate
    Wire.beginTransmission(MPU6050);
    Wire.write(0x3B);             //Access value register
    Wire.endTransmission(false);  //End connection while retaining access
    Wire.requestFrom(MPU6050, 6, true);
    float readings[3]; //Y, X, Z
     
    for(int j = 0; j < 3; j++)
    {
      readings[j] = readAcc() + offsets[i][j];
    }
  
    //Compute for Roll & Pitch
    values[i*2] = atan(readings[0] / sqrt( pow(readings[1], 2) + pow(readings[2], 2))) * 180/PI;  //Roll
    values[i*2+1] =  atan(-1 * readings[2] / sqrt( pow(readings[0], 2) + pow(readings[1], 2))) * 180/PI;  //Pitch
  }

  return values;
}


//Read accelerometer values
float Reader::readAcc()
{
  //Accelerometer value is 10 bits but can only send 8 bits at a time, 
  //so 1st Wire.read() is 1st 8 bits, then 2nd Wire.read() is for the last 2 bits
  float value = Wire.read() << 8 | Wire.read();
  return value/16384; //To limit range to +- 2g
}

//Switch channel of MUX
void Reader::switchChannel(int channel)
{
  if (channel > 7) return;
  //Write value in MUX in 8 bits that must have only 1 high bit
  //The position of the high bit is the channel connected by MUX
  //ex: 00100000 means channel 5
  Wire.beginTransmission(MUX);
  Wire.write(1 << channel);
  Wire.endTransmission();
}
