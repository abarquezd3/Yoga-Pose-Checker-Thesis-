#include "SearchTree.h"
#include <Wire.h>

#define OUTPUT_PIN 8

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

float values[8]; //Rol, Pitch
//1 acc value = 3 digits + 2 decimals + '.' + '-' = 7 bytes
//2 acc values = 7 * 2 = 14 bytes
//2 ',' between acc value = 2 bytes (2 + 14) = 16 total bytes
//4 accelerometers = 16*4 = 64
//Add end value = 65 total bytes, + 5 bytes for good luck = 70 bytes
char str[70] = "";

const int MPU6050 = 0x68,
          MUX = 0x70;

SearchTree searcher;

void setup() {
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

  //Setup Serial
  Serial.begin(9600);
}

void loop()
{
  memset(str, 0, sizeof(str));
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
  
    appendFloat(values[i*2], str);
    strcat(str, ",");

    appendFloat(values[i*2+1], str);
    strcat(str, ",");
  }
  Serial.print(str);

  if(searcher.search(values))
  {
    Serial.print("true\n");
    digitalWrite(OUTPUT_PIN, HIGH);
  } else 
  {
    Serial.print("false\n");
    digitalWrite(OUTPUT_PIN, LOW);
  }
  
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

//Trim char[]
void trimCh(char *str)
{
  char *p = strchr(str, ' '); //Search for space
  if(p) *p = 0; //Terminate
}

//Append float to char[]
void appendFloat(float value, char* string)
{
   //Get (3 max whole number with 2 decimal point value (6 bytes) + 
   // '-' in case of negative + end point = 8 bytes
   char valueStr[8];
   dtostrf(value, -7, 2, valueStr); //Convert float to char[]
   trimCh(valueStr);                //Trim extra spaces
   strcat(string, valueStr);        //Concat string 
}
