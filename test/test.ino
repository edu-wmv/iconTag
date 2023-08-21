#include "functions.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  ethernetUDP();
  rfid.PCD_Init();
  lcd.begin(16, 2);

  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}

void loop() {
  tagReader();
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("ICON");
}

