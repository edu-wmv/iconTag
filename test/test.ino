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

