#include <SPI.h>
#include <MFRC522.h>
#include <SD.h>
#include "getTime.h"
#include "functions.h"

File tagFile;
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag63

  while(Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(1000);
  }
  Serial.print("Ethernet Shield IP (DHCP): ");
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort);
  Serial.println("Ethernet UDP Start....");

  // SD CARD INITIALIZATION
  while(!Serial);
  Serial.print("Initializing SD card...");
  SD.begin(4);
  if (!SD.begin(4)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this serial monitor after fixing your issue!");
    while (1);
  }
  Serial.println("Initialization done.");

  // CHECAR SD E ETHERNET SHIELD EM CONJUNTO

  setSyncProvider(getNtpTime);

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
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : ":"); // FIX
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      
      printTime(hour(), minute(), second());

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
}


