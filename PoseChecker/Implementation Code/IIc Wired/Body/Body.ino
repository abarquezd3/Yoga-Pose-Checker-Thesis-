#include <Wire.h>

const int offsets[3] = {
  0.00,         //xOffset
  0.00,         //yOffset
  0.00          //zOffset
};

const int ADXL345 = 0x53,
          MUX = 0x70,
          ACCBUS = 2;

//1 acc value = 3 digits + 2 decimals + '.' + '-' = 7 bytes
//2 acc values = 7 * 2 = 14 bytes
//2 ',' between acc value = 2 bytes (2 + 14) = 16 total bytes
//4 accelerometers = 16*4 = 64
//Add end value = 65 total bytes, + 5 bytes for good luck = 70 bytes
char str[70] = "";

void setup() {
  Serial.begin(9600);
  
  //Setup I2C connection
  Wire.begin();
  //Move to channel 2 (Accelerometer location)
  switchChannel(ACCBUS);
  //Set ADXL345 to Measuring Mode
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);   //Access Power Register
  Wire.write(8);      //Write for Measuring mode
  Wire.endTransmission();
  delay(10);
}

void loop()
{
  //Switch to accelerometer
  switchChannel(ACCBUS);
  //Communicate
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);             //Access value register
  Wire.endTransmission(false);  //End connection while retaining access
  Wire.requestFrom(ADXL345, 6, true);
  int readings[3]; //X, Y, Z
   
  for(int i = 0; i < 3; i++)
  {
    readings[i] = readAcc() + offsets[i];
  }

  //Compute for Roll & Pitch
  int values[2] = {
    atan(values[1] / sqrt( pow(values[0], 2) + pow(values[2], 2))) + 180/PI,      //Roll
    atan(-1 * values[2] / sqrt( pow(values[1], 2) + pow(values[0], 2))) * 180/PI  //Pitch
  };

  //Print
  memset(str, 0, sizeof(str));
  //Get the 2 values
  for(int i = 0; i < 2; i++)
  {
    //Get 7 bytes for acc value + end pointer
    char valString[8];
    dtostrf(values[i], -7, 2, valString);
    trimCh(valString);
    strcat(str, valString);
    strcat(str, ",");
  }
  strcat(str, "0,0,0,0,0,0,");
  Serial.println(str);
 
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

//Trim char[]
void trimCh(char *str)
{
  char *p = strchr(str, ' '); //Search for space
  if(p) *p = 0; //Terminate
}
