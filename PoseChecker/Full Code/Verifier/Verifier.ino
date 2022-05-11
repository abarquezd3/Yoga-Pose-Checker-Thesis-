#include "SearchTree.h"

#include "nRF24L01.h"
#include "RF24.h"
#include "SPI.h"

#define OUTPUT_PIN 8  //Feel free to edit this

//Address : 250
const uint64_t address = 250;
//Addresses
const int ACC = 0x68, //ADXL35 ADDRESS - Accelerometer
          MUX = 0x70; //TCA9548A ADDRESS - I2X MUX
          
//ORDER: Torso, Arm, Left Thigh, Right Thigh
const int channels[4] = {2, 7, 3, 6};
//String used to communicate /w Pose Generator
//(5 values per float /w "." & "-" + ",") = 8 bytes
// 2 floats per accelerometer (roll & pitch) = 16 bytes
// 4 accelerometers = 64 bytes + "end point" = 65 bytes
// +5 extra values just in case = 70 bytes 
char serialString[70] = "";
// 2 values from roll & pitch * 4 accelerometers = 8 values
float values[8];

RF24 radio(9, 10);
SearchTree searcher;

void setup() {
  Serial.begin(9600);

  //Radio
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {  
  if(radio.available())
  {
    //Read Values
    radio.read(&values, sizeof(values));
    memset(serialString, 0, sizeof(serialString)); //Clean serial String to ""
    //Convert to serial string
    for(int i = 0; i < 8; i++)
    {
      appendFloat(values[i], serialString);
      strcat(serialString, ",");
    }
    
    Serial.print(serialString);
    if(searcher.search(values))
    {
      Serial.print("true\n");
      digitalWrite(OUTPUT_PIN, HIGH); 
    }
    else
    {
      Serial.print("false\n");
      digitalWrite(OUTPUT_PIN, LOW); 
    }
  }

  delay(300);
}

//Sub Functions
//Trims the white spaces of string
void trimCh(char *str)
{
  //Find 1st space
  char *p = strchr(str, ' ');
  if(p) *p = 0; //Set to end pointer
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
