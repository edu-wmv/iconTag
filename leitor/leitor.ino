#include "functions.h"

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag63

  ethernetUDP();
  setSyncProvider(getNtpTime);

	Serial.println("Aproxime a tag do leitor");

  //codigo do lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Aproxime a tag");
  Serial.println("OK");
}

void loop() {
  tagReader();
}