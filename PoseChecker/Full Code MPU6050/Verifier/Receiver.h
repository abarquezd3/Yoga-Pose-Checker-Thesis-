#ifndef RECEIVER_H
#define RECEIVER_H

#include<Arduino.h>
#include<stdlib.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

class Receiver
{
  public:
    Receiver(uint64_t address);
    float* receive();
    bool isAvailable();
    char* getSerialString();
    float* getValues();
  private:
    void trimCh(char *str); //Trims empty spaces on the right
    void appendFloat(float value, char* string);  //Appends float to char[]
    
    RF24 radio;
    // 2 values from roll & pitch * 4 accelerometers = 8 values
    float values[8];
    //String used to communicate /w Pose Generator
    //(5 values per float /w "." & "-" + ",") = 8 bytes
    // 2 floats per accelerometer (roll & pitch) = 16 bytes
    // 4 accelerometers = 64 bytes + "end point" = 65 bytes
    // +5 extra values just in case = 70 bytes 
    char serialString[70] = ""; 
};

#endif
