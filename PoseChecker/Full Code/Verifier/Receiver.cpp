#include "Receiver.h"

Receiver::Receiver(uint64_t address)
{
  radio = RF24(9, 10);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
}

float* Receiver::receive()
{
  radio.read(&values, sizeof(values));
  memset(serialString, 0, sizeof(serialString)); //Clean serial String to ""
  //Convert to serial string
  for(int i = 0; i < 8; i++)
  {
    appendFloat(values[i], serialString);
    strcat(serialString, ",");
  }
  return values;
}

char* Receiver::getSerialString()
{
  return serialString;
}

float* Receiver::getValues()
{
  return values;
}

bool Receiver::isAvailable()
{
  return radio.available();
}

//Trims the white spaces of string
void Receiver::trimCh(char *str)
{
  //Find 1st space
  char *p = strchr(str, ' ');
  if(p) *p = 0; //Set to end pointer
}

//Appends float to char[]
void Receiver::appendFloat(float value, char* string)
{
   //Get (3 max whole number with 2 decimal point value (6 bytes) + 
   // '-' in case of negative + end point = 8 bytes
   char valueStr[8];
   dtostrf(value, -7, 2, valueStr); //Convert float to char[]
   trimCh(valueStr);                //Trim extra spaces
   strcat(string, valueStr);        //Concat string
}
