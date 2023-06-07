#include <SPI.h>
#include <MFRC522.h>
#include <variables.h>

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag

	Serial.println("Aproxime a tag do leitor");
}

void loop() {

  // tag disponível
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
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }

}
