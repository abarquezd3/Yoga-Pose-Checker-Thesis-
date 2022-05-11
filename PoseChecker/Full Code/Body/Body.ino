#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"
#include<Wire.h>

//Channels for each accelerometer
const int buses[4] {
  0,  //Torso
  0,  //Arm
  0,  //Left Thigh
  0   //Right Thigh
};

const float offsets[4][3] = {
  0.00, 0.00, 0.00,        //y, x, z Offset for Torso
  0.00, 0.00, 0.00,        //y, x, z Offset for Arm
  0.00, 0.00, 0.00,        //y, x, z Offset for Left Thigh
  0.00, 0.00, 0.00,        //y, x, z Offset for Right Thigh
};

//Arbitrary Address that should be shared by Receiver & Transmitter
const uint64_t address = 250;

RF24 radio(9,10);
float values[8];
String base = "";
const int ACC = 0x53,
          MUX = 0x70;

void setup() {
  Serial.begin(9600);

  Wire.begin();
  //Configure each accelerometer
  for(int i = 0; i > 4; i++)
  {
    //Set acc to reading mode
    switchChannel(buses[i]);
    //Set ADXL345 to Measuring Mode
    Wire.beginTransmission(ACC);
    //Set ADXL345 to Measuring Mode
    Wire.write(0x2D); //Access Power Register
    Wire.write(8); //Write for Measuring mode
    Wire.endTransmission();
    delay(10);
  }   
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  for(int i = 0; i < 4; i++)
  {
    //Switch I2C Channel
    switchChannel(buses[i]);
    //Communicate to ACC in Channel
    Wire.beginTransmission(ACC);
    Wire.write(0x32);
    Wire.endTransmission(false);
    Wire.requestFrom(ACC, 6, true); //6 Values, 3 axes (X,Y,Z) * 2 parameters of each axis(X,Y)
    float reading[3];
     
    for(int j = 0; j < 3;j++) //Fill in X,Y,Z to array
    {
      reading[j] = readAccValue() + offsets[i][j];
    }
    //Roll & Pitch formula is derived by multiplying the scalar of Accelerometer values to the
    //3D Rotation Matrix then solve for the formula of the 2 angles, roll & pitch
    float roll = atan(reading[1]/ sqrt(pow(reading[0], 2) + pow(reading[2], 2))) * 180/PI;
    values[i * 2] = roll;
    float pitch = atan(-1 * reading[0]/ sqrt(pow(reading[1], 2) + pow(reading[2], 2))) * 180/PI;
    values[i * 2 + 1] = pitch;
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

//Reads Accelerometer value
float readAccValue()
{
  //Accelerometer value is 10 bits but can only send 8 bits at a time, 
  //so 1st Wire.read() is 1st 8 bits, then 2nd Wire.read() is for the last 2 bits
  float value = Wire.read() | Wire.read() << 8;
  return value/256; //To limit range to +- 2g
}
