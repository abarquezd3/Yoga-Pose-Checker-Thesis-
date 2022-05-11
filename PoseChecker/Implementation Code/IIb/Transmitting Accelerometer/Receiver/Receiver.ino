#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

const uint64_t address = 250;

RF24 radio = RF24(9, 10);
float values[3]; 
//1 acc value = 3 digits + 2 decimals + '.' + '-' = 7 bytes
//3 acc values = 7 * 3 = 21 bytes
//3 ',' between acc value = 3 bytes (3 + 21) = 24 total bytes
//Add end value = 25 total bytes, + 3 bytes for good luck = 28 bytes
char str[28] = "";

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
     //Get the 3 values
     for(int i = 0; i < 3; i++)
     {
      //Get 7 bytes for acc value + end pointer
      char valString[8];
      dtostrf(values[i], -7, 2, valString);
      trimCh(valString);
      strcat(str, valString);
      strcat(str, ", ");
     }
     Serial.println(str);
  }
}

//Trim char[]
void trimCh(char *str)
{
  char *p = strchr(str, ' '); //Search for space
  if(p) *p = 0; //Terminate
}
