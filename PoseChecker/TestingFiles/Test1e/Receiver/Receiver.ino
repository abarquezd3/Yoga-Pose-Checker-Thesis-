#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const uint64_t address = 250;

RF24 radio = RF24(9, 10);
char str[180] = "";

void setup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if(radio.available())
  {
     memset(str, 0, sizeof(str));
     while(radio.available())
     {
       float values[8] = {0};      
       radio.read(&values, sizeof(values));
       for(int i = 0; i < 8; i++)
       {
        appendFloat(values[i], str);
        strcat(str, ", ");
       }
     }

     Serial.println(str);
  }
}

//Appends float to char[]
void appendFloat(float value, char* string)
{
   //Get (3 max whole number with 2 decimal point value (6 bytes) + 
   // '-' in case of negative + end point = 8 bytes
   char valueStr[8];
   dtostrf(value, -7, 2, valueStr); //Convert float to char[]
   trimCh(valueStr);                //Trim extra spaces
   strcat(string, valueStr);        //Concat string
}

void trimCh(char *str)
{
  //Find 1st space
  char *p = strchr(str, ' ');
  if(p) *p = 0; //Set to end pointer
}
