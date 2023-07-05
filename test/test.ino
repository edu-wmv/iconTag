#include "functions.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("INIT");

  Serial.println("Aproxime a tag do leitor: ");
}

void loop() {
  tagReader();
}