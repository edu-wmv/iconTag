#include "variables.h"

// LED FUNCTIONS
void redOn() {
  digitalWrite(red, HIGH);
}

void greenOn() {
  digitalWrite(green, HIGH);
}

void blueOn() {
  digitalWrite(blue, HIGH);
}

void whiteOn() {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
}

void cianOn() {
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
}

void ledOff() {
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

void ethernetUDP() {
  while(Ethernet.begin(mac) == 0) {
    blueOn();
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(1000);
  }
  Serial.print("Ethernet Shield IP (DHCP): ");
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort);
  greenOn();
  Serial.println("Ethernet UDP Start....");
}

void sendData(String uid) {
  Serial.println("Sending data...");
  Serial.println(uid);
}

void tagReader() {
  String uid_record;
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {
      for ( int i = 0; i < rfid.uid.size; i++) {
        uid_record.concat(rfid.uid.uidByte[i]);
      }
      
      sendData(uid_record);

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      delay(2500);
    }
  }
}

