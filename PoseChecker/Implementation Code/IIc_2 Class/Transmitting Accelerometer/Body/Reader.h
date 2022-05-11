#ifndef READER_H
#define READER_H

#include <Arduino.h>
#include <Wire.h>

class Reader
{
  public:
    Reader();
    void begin();
    void readValues(float values[8]);
  private:
    //Channels for each accelerometer
    const int buses[4] {
      0,  //Torso
      0,  //Arm
      0,  //Left Thigh
      0   //Right Thigh
    };
  
    const float offsets[4][3] = {
      {0.00, 0.00, 0.00},        //y, x, z Offset for Torso
      {0.00, 0.00, 0.00},        //y, x, z Offset for Arm
      {0.00, 0.00, 0.00},        //y, x, z Offset for Left Thigh
      {0.00, 0.00, 0.00}        //y, x, z Offset for Right Thigh
    };

    void switchChannel(int channel);
    float readAcc();
    float readings[3]; //X, Y, Z
    //Addresses
    const int MPU6050 = 0x68, //ADXL35 ADDRESS - Accelerometer
              MUX = 0x70; //TCA9548A ADDRESS - I2X MUX
};

#endif
