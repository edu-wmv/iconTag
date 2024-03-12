#include "functions.h"

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag63

  lcd.init();
  lcd.backlight();

  ethernetUDP();
  setSyncProvider(getNtpTime);

  lcd.clear();
  lcd.setCursor(0, 0);
  printOnCenter("iconTAG");
  timeGet();
}

void loop() {
  //...Display Time to LCD...
  timeShow();
  tagReader();
}