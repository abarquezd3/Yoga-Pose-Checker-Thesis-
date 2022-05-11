#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const uint64_t address = 250;

RF24 radio = RF24(9, 10);
char str[96] = "";


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
     float values[8] = {0};
     radio.read(&values, sizeof(values));

     for(int i = 0; i < 8; i++)
     {
      char value[8];
      dtostrf(values[i], -7, 2, value); //Convert float to char[]
      trimCh(value);                    //Trim extra spaces
      strcat(str, value);               //Concat string
      strcat(str, ", ");
     }
     
     Serial.println(str);
  }
}

void trimCh(char *str)
{
  //Find 1st space
  char *p = strchr(str, ' ');
  if(p) *p = 0; //Set to end pointer
}
