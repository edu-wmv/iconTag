#include "variables.h"

// ETHERNET UDP CONNECTION
void ethernetUDP() {
  while(Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro de conexao");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reconectando...");
    delay(7000);
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
void registerNewUser() {
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    client.println("POST /api/v1/add_user HTTP/1.1");
    client.println("Host: " + String(HOST) + ":" + String(HTTP_PORT));
    client.println("Authorization: " + String(API_KEY));
    client.println("name: " + String(name));
    client.println("uid: " + String(uid));
    client.println("Connection: close");
    client.println();

    char payload[200] = "";

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

    Serial.println(payload);

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    String statusCode = doc["statusCode"];
    String message = doc["message"];
    String code = doc["code"];

    if (code == "user_created") {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Cadastro feito!");
    } else if (code == "user_already_exists") {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Iconico existe");
    } else if (code == "internal_error") {
      lcd.clear();
      lcd.setCursor(0, 0);
      Serial.println("Erro interno do servidor");
      Serial.print(message);
      printOnCenter("Erro de conexao");
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Erro de conexao");
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

void updateUser() {
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    client.println("GET /get_user HTTP/1.1");
    client.println("Host: " + String(HOST) + ":" + String(HTTP_PORT));
    client.println("Authorization: " + String(API_KEY));
    client.println("userId: " + String(user_id));
    client.println("Connection: close");
    client.println();

    char payload[200] = "";

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

    Serial.println(payload);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    String statusCode = doc["statusCode"];
    String message = doc["message"];
    String code = doc["code"];
    String user = doc["user"];

    if (code == "success") {
      Serial.println("Usuario encontrado");
      Serial.println(user);
    } else if (code == "not-found") {
      Serial.println("Usuario nao existe");
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