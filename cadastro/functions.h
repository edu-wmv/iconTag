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

void printOnCenter(String text) {
  int size = text.length();
  int spaces = float(16 - size) / 2;
  spaces = ceil(spaces);
  for (int i = 0; i < spaces; i++) {
    lcd.print(" ");
  }
  lcd.print(text);
}

// HTTP POST REQUEST
void sendData() {
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    client.println("POST /insertData HTTP/1.1");
    client.println("Host:" + String(HOST) + ":" + String(HTTP_PORT));
    client.println("api-key: " + String(API_KEY));
    client.println("name:" + String(name));
    client.println("uid:" + String(uid));
    client.println("Connection: close");
    client.println();

    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        if (c == '{') {
          isData = true;
        }
        if (isData) {
          payload[strlen(payload)] = c;
        }
        if (c == '}') {
          isData = false;
        }
      }
    }

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    String statusCode = doc["statusCode"];
    String message = doc["message"];
    String code = doc["code"];

    if (code = 'sucess') {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Cadastro feito!");
    } else if (code = 'already-exists') {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Iconico existe");
    }

    client.stop();
    client.flush();
  } else {
    Serial.println("connection failed");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ERRO DE CONEXAO");
    delay(3000);
    lcd.clear();
  }
}

// RFID TAG READER
String tagReader() {
  // tag disponível
  String uid_record;
  while(1)
  if (rfid.PICC_IsNewCardPresent()) {
    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {

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
    }
  }
}