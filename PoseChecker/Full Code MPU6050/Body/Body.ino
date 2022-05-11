#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"
#include <Wire.h>

//Arbitrary Address that should be shared by Receiver & Transmitter
const uint64_t address = 250;
//Channels for each accelerometer
const int buses[4] {
  6,  //Torso
  1,  //Arm
  4,  //Left Thigh
  7   //Right Thigh
};

const float offsets[4][3] = {
  0.00, 0.00, 0.00,        //y, x, z Offset for Torso
  0.00, 0.00, 0.00,        //y, x, z Offset for Arm
  0.00, 0.00, 0.00,        //y, x, z Offset for Left Thigh
  0.00, 0.00, 0.00,        //y, x, z Offset for Right Thigh
};

float values[8];
RF24 radio(9,10);
String base = "";
const int MPU6050 = 0x68,
          MUX = 0x70;

void setup() {
  Serial.begin(9600);

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
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();

}

void loop() {
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
    Serial.print(base + values[i*2] + "," + values[i*2+1] + ",");
  }
  Serial.println();
  
  radio.write(&values, sizeof(float[8]));
  delay(300);
}

void switchChannel(int bus)
{
  if (bus > 7) return;
  //Write value in MUX in 8 bits that must have only 1 high bit
  //The position of the high bit is the channel connected by MUX
  //ex: 00100000 means channel 5
  Wire.beginTransmission(MUX);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

float readAcc()
{
  //Accelerometer value is 10 bits but can only send 8 bits at a time, 
  //so 1st Wire.read() is 1st 8 bits, then 2nd Wire.read() is for the last 2 bits
  float value = Wire.read() << 8 | Wire.read();
  return value/16384; //To limit range to +- 2g
}
