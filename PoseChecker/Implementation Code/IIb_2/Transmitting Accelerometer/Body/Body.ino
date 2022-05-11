#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

const int offsets[3] = {
  0.00,         //xOffset
  0.00,         //yOffset
  0.00          //zOffset
};

const int MPU6050 = 0x68,
          MUX = 0x70,
          ACCBUS = 2;
RF24 radio(9, 10); //CE, CSN

const uint64_t address = 250; 
const String base = "";

void setup() {
  //Setup Wireless Transceiver
  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();

  //Setup I2C connection
  Wire.begin();
  //Move to channel 2 (Accelerometer location)
  switchChannel(ACCBUS);
  //Set ADXL345 to Measuring Mode
  Wire.beginTransmission(MPU6050);
  Wire.write(0x6B);   //Access Power Register
  Wire.write(0x00);      //Write for Measuring mode
  Wire.endTransmission(true);
  calculateErrors();
}

void loop()
{
  Serial.print("heello");
  //Switch to accelerometer
  switchChannel(ACCBUS);
  //Communicate
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);             //Access value register
  Wire.endTransmission(false);  //End connection while retaining access
  Wire.requestFrom(MPU6050, 6, true);
  int values[3];  //X, Y, Z
  for(int i = 0; i < 3; i++)
  {
    values[i] = readAcc() + offsets[i];
  }
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
  float value = Wire.read() | Wire.read() << 8;
  return value/256; //To limit range to +- 2g
}

//Tests from 200 samples
void calculateErrors()
{
  int c = 0;
  float accX, accY, accZ,
        accErrX, accErrY;
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
