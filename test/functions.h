#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>
// RFID MODULE PINS
#define RST_PIN   9 // RFID reset pin
#define SS_PIN    8 // RFID pin
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

const int rs = 31, en = 33, d4 = 43, d5 = 45, d6 = 47, d7 = 49;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void tagReader() {
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    Serial.println("1");

    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {

      Serial.println("OK");

      lcd.print("BEM VINDO AO ICON!");

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}