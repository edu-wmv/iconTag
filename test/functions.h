#include <MFRC522.h>
#include <SPI.h>

// RFID MODULE PINS
#define RST_PIN   9 // RFID reset pin
#define SS_PIN    8 // RFID pin
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

void tagReader() {
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    Serial.println("1");

    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {

      Serial.println("OK");

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}