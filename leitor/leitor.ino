#include "functions.h"

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag63

  ethernetUDP();
  setSyncProvider(getNtpTime);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("iconTAG");
  timeGet();
}

void loop() {
  //...Display Time to LCD...
  timeShow();
  tagReader();
}