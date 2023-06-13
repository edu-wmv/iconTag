#include "functions.h"

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag63

  ethernetUDP();
  setSyncProvider(getNtpTime);

	Serial.println("Aproxime a tag do leitor");
}

void loop() {
  tagReader();
}

void tagReader() {
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    String time = timeFix(hour(), minute(), second());
    String uid;
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
        uid.concat(rfid.uid.uidByte[i]);
      }
      
      Serial.print("\nTime: ");
      Serial.println(time);
      Serial.print("UID Geral: ");
      Serial.println(uid);

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}
