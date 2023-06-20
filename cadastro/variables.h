#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

// RFID MODULE PINS
#define RST_PIN   9 // RFID reset pin
#define SS_PIN    8 // RFID pin
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

// ETHERNET & TIME VARIABLES
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED }; // end. mac
unsigned int localPort = 8888; // porta a ser usada no protcolo de rede
EthernetUDP Udp; // cria o protocolo iniciador do UDP
EthernetClient client;

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

int HTTP_PORT = 8080;
byte HOST_NAME[] = { 192, 168, 0, 123 };

String uid;
String name;
String query;

void sendData() {

  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    client.println("GET /insertData HTTP/1.1");
    client.println("name:" + String(name));
    client.println("uid:" + String(uid));
    client.println("Host: 192.168.0.123:8080");
    client.println("api-key: SUNPTl9pY29udGFn");
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
