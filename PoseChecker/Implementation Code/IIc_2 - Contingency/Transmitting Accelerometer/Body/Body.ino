#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

//Channels for each accelerometer
const int buses[4] {
  0,  //Torso
  0,  //Arm
  0,  //Left Thigh
  0   //Right Thigh
};

//index of adxlbus
const int adxlbus = 3; //(Right Thigh)

const float offsets[4][3] = {
  0.00, 0.00, 0.00,        //y, x, z Offset for Torso
  0.00, 0.00, 0.00,        //y, x, z Offset for Arm
  0.00, 0.00, 0.00,        //y, x, z Offset for Left Thigh
  0.00, 0.00, 0.00,        //y, x, z Offset for Right Thigh
};

String base = "";
const int MPU6050 = 0x68,
          ADXL345 = 0x53,
          MUX = 0x70;
RF24 radio(9, 10); //CE, CSN

const uint64_t address = 250;
float values[8];

void setup() {
  Serial.begin(9600);
  
  //Setup Wireless Transceiver
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();

  //Setup I2C connection
  Wire.begin();
  for(int i = 0; i < 4; i++)
  {
    //Move to channel 2 (Accelerometer location)
    switchChannel(buses[i]);

    if(i != adxlbus) {
      //Reset Connections MPU6050
      Wire.beginTransmission(MPU6050);
      Wire.write(0x6B);
      Wire.write(0x00);
      Wire.endTransmission(true);      
    } else 
    {
      //Set ADXL345 to Measuring Mode
      Wire.beginTransmission(ADXL345);
      Wire.write(0x2D);   //Access Power Register
      Wire.write(8);      //Write for Measuring mode
      Wire.endTransmission();
    }

    delay(10);
  }
}

void loop()
{
  for(int i = 0; i < 4; i++)
  {
    //Switch to accelerometer
    switchChannel(buses[i]);

    float readings[3]; //Y, X, Z   
    if(i != adxlbus)
    {
      //Communicate
      Wire.beginTransmission(MPU6050);
      Wire.write(0x3B);             //Access value register
      Wire.endTransmission(false);  //End connection while retaining access
      Wire.requestFrom(MPU6050, 6, true);
    } else
    {
      Wire.beginTransmission(ADXL345);
      Wire.write(0x32);             //Access value register
      Wire.endTransmission(false);  //End connection while retaining access
      Wire.requestFrom(ADXL345, 6, true);
    }

    for(int j = 0; j < 3; j++)
    {
      readings[j] = ( (i != adxlbus) ? readAcc() : readAccADXL() ) + offsets[i][j];
    }

    //Compute for Roll & Pitch
    if(i != adxlbus)
    {
      values[i*2] = atan(readings[0] / sqrt( pow(readings[1], 2) + pow(readings[2], 2))) * 180/PI;  //Roll
      values[i*2+1] =  atan(-1 * readings[2] / sqrt( pow(readings[0], 2) + pow(readings[1], 2))) * 180/PI;  //Pitch      
    } else 
    {
      values[i*2] = atan(values[1] / sqrt( pow(values[0], 2) + pow(values[2], 2))) + 180/PI;  //Roll
      values[i*2+1] = atan(-1 * values[2] / sqrt( pow(values[1], 2) + pow(values[0], 2))) * 180/PI;  //Pitch   
    }
  
    Serial.print(base + values[i*2] + "," + values[1*2+1] + ",");
  }
  Serial.println();

  radio.write(&values, sizeof(values)); //Send data
  delay(100);
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

float readAccADXL()
{
  //Accelerometer value is 10 bits but can only send 8 bits at a time, 
  //so 1st Wire.read() is 1st 8 bits, then 2nd Wire.read() is for the last 2 bits
  float value = Wire.read() | Wire.read() << 8;
  return value/256; //To limit range to +- 2g
}
