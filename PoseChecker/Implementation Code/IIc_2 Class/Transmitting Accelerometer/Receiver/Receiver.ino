#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const uint64_t address = 250;

RF24 radio = RF24(9, 10);
float values[8]; //Rol, Pitch
//1 acc value = 3 digits + 2 decimals + '.' + '-' = 7 bytes
//2 acc values = 7 * 2 = 14 bytes
//2 ',' between acc value = 2 bytes (2 + 14) = 16 total bytes
//4 accelerometers = 16*4 = 64
//Add end value = 65 total bytes, + 5 bytes for good luck = 70 bytes
char str[70] = "";

void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if(radio.available())
  {
     memset(str, 0, sizeof(str));
     radio.read(&values, sizeof(values));
           
     //Get the 2 values
     for(int i = 0; i < 8; i++)
     {
      appendFloat(values[i], str);
      strcat(str, ",");
     }
     //strcat(str, "0,0,0,0,0,0,");
     Serial.println(str);
  }
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
