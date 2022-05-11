#ifndef READING_H
#define READING_H

#include <Arduino.h>
#include <Wire.h>

class Reading 
{
  public:
    Reading();
    float* readValues(int ACCBUS);
    void switchChannel(int channel);
    void readChannels();
  private:
    float calibrations[2][3] {
      {0.00, 0.00, 0.00}, //Torso
      {0.00, 0.00, 0.00}  //Arm
    };
    float readAcc();
    float values[5];
    const int MPU6050 = 0x68,
              MUX = 0x70;
    //Torso, Arm, Left Thigh
    const int channel[3] = {6,7, 4};
};
#endif
