#include "Reading.h"

const String base = "";
const int channel = 7;
float* vals;

Reading r;

void setup() {
  Serial.begin(9600);
  //UNCOMMENT TO TEST CONNECTIONS
  //r.readChannels();
}

void loop() {
  vals = r.readValues(channel);
  String output = "";
  for(int i = 0; i < 5; i++)
  {
    output += vals[i];
    output += ",";
  }
  Serial.println(output);
  delay(100);
}
