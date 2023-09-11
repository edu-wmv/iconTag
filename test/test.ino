<<<<<<< HEAD
#include "functions.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  ethernetUDP();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
}

void loop() {
  tagReader();
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("ICON");
}

=======
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
>>>>>>> 8c7b150ddda5b350f59fb65decfc94e199254ab6
