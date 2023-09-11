#include <TimeLib.h>
#include "functions.h"

void setup(){
  Serial.begin(9600);
  ethernetUDP();
  setSyncProvider(getNtpTime);
}

void loop(){
    Serial.print(weekday());
}