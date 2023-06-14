#include <SPI.h>
#include <MFRC522.h>

// RFID MODULE PINS
#define RST_PIN   9 // RFID reset pin
#define SS_PIN    8 // RFID pin
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

String tagReader() {
  // tag disponível
  Serial.println("Ready");
  String uid_record;
  while(1)
  if (rfid.PICC_IsNewCardPresent()) {
    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {
      // esse código cria a variavél para armazenar as infos do tipo da tag
      MFRC522::PICC_Type picc_Type = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(picc_Type));

      // retorna UID da tag em HEX
      Serial.print("UID: ");
      for ( int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : ":"); // FIX
        Serial.print(rfid.uid.uidByte[i], HEX);
        uid_record.concat(rfid.uid.uidByte[i]);
      }
      
      Serial.print("\nUID Geral: ");
      Serial.println(uid_record);

      return uid_record;

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}

void _register() {
  tagReader();
}