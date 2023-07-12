#include "variables.h"

// ETHERNET UDP CONNECTION
void ethernetUDP() {
  while(Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(1000);
  }
  Serial.print("Ethernet Shield IP (DHCP): ");
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort);
  Serial.println("Ethernet UDP Start....");
}

// RFID TAG READER
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

// HTTP GET REQUEST
void sendData() {

  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    client.println("POST /insertData HTTP/1.1");
    client.println("name:" + String(name));
    client.println("uid:" + String(uid));
    client.println("Host:" + String(HOST_NAME) + ":" + String(HTTP_PORT));
    client.println("api-key: " + String(API_KEY));
    client.println("Connection: close");
    client.println();

    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {
    Serial.println("connection failed");
  }
}