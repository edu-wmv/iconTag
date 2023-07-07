#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#define RST_PIN   9 // RFID reset pin
#define SS_PIN    8 // RFID pin
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

const int rs = 48, en = 46, d4 = 45, d5 = 43, d6 = 41, d7 = 39;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void tagReader() {
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    Serial.println("1");

    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {

      Serial.println("OK");
      lcd.setCursor(16,0);
      lcd.print("BEM VINDO AO ICON");
      for (int poslcd = 0; poslcd < 36; poslcd++)
      {
        lcd.setCursor(16,1);
        lcd.scrollDisplayLeft();
        delay (250);
      }
      lcd.clear();

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}