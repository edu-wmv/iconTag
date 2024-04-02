#include "variables.h"

void ethernetUDP() {
  while(Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro de conexao");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reconectando...");
    delay(7000);
  }
  Serial.println("Ethernet Shield IP (DHCP): ");
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort);
  Serial.println("Ethernet UDP Start....");
}

// TIME SEND & REQUEST FUNCTIONS
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime() {
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// FORMAT TIME FUNCTION
String dateFix(int y, int mon, int day, int h, int min, int s) {
  char buffer[19];
  sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d", y, mon, day, h, min, s);
  return buffer;
}

String hourFix(int h, int m, int s) {
  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", h, m, s);
  return buffer;
}

void printOnCenter(String name) {
  int size = name.length();
  int spaces = float(16 - size) / 2;
  spaces = ceil(spaces);
  for (int i = 0; i < spaces; i++) {
    lcd.print(" ");
  }
  lcd.print(name);
}

void send(String data, String token) {
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    String command = "POST /api/v2/icontag/add_point HTTP/1.1";

    delay(1000);
    Serial.print(command + "\r\n" +
                 "Host: " + String(HOST) + ":" + String(HTTP_PORT) + "\r\n" +
                 "Content-Type: application/json" + "\r\n" +
                 "Authorization: Bearer " + token + "\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "Connection: close" + "\r\n" + "\r\n" +
                 data + "\r\n");
    client.print(command + "\r\n" +
                 "Host: " + String(HOST) + ":" + String(HTTP_PORT) + "\r\n" +
                 "Content-Type: application/json" + "\r\n" +
                 "Authorization: Bearer " + String(token) + "\r\n" +
                 "Content-Length: " + data.length() + "\r\n" +
                 "Connection: close" + "\r\n" + "\r\n" +
                 data + "\r\n");
    client.println();
                 
    char payload[200] = "";
    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        Serial.print(c);
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

    // Serial.println(payload);
    DynamicJsonDocument doc(1024);  
    deserializeJson(doc, payload);
    String message = doc["message"];
    String code = doc["code"];
    String userName = doc["username"];

    if (code == "user_not_found" or code.length() == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Nao encontrado");
    } else if (code == "welcome") {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter(userName);
      lcd.setCursor(0, 1);
      String data = "Entrada " + String(time); 
      lcd.print(data);
    } else if (code == "bye") {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter(userName);
      lcd.setCursor(0, 1);
      String data = "Saida " + String(time); 
      lcd.print(data);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      printOnCenter("Erro desconhecido");
    }


    client.stop();
    client.flush();
    memset(payload, 0, sizeof(payload));
    Serial.println("disconnected");
  } else {
    Serial.println("Connection failed :/");
  }
}

void getJwt() {
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("connected");

    String command = "POST /api/v2/auth/login HTTP/1.1";
    String postData = "{\"username\":\"" + String(USERNAME) + "\",\"password\":\"" + String(PASSWORD) + "\"}";

    Serial.println(command + "\r\n" +
                 "Host: " + String(HOST) + ":" + String(HTTP_PORT) + "\r\n" +
                 "Content-Type: application/json" + "\r\n" +
                 "Content-Length: " + postData.length() + "\r\n" +
                 "Connection: close" + "\r\n" + "\r\n" +
                 postData + "\r\n");

    client.print(command + "\r\n" +
                 "Host: " + String(HOST) + ":" + String(HTTP_PORT) + "\r\n" +
                 "Content-Type: application/json" + "\r\n" +
                 "Content-Length: " + postData.length() + "\r\n" +
                 "Connection: close" + "\r\n" + "\r\n" +
                 postData + "\r\n");

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

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);
  String username = doc["username"];
  String _token = doc["token"];
  String expires = doc["expiresAt"];
  token = _token;
  Serial.println("Token set!");

  client.stop();
  client.flush();
  } else {
    Serial.println("Connection failed :/");
  }
}

void tagReader() {
  // tag disponível
  // Serial.println("Tag disponível");
  if (rfid.PICC_IsNewCardPresent()) {
    fullTime = dateFix(year(), month(), day(), hour(), minute(), second());
    time = hourFix(hour(), minute(), second());
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Identificando...");
    delay(200);

    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {
      for ( int i = 0; i < rfid.uid.size; i++) {
        uid.concat(rfid.uid.uidByte[i]);
      }

      String postData = "{\"uid\":\"" + String(uid) + "\",\"time\":\"" + String(fullTime) + "\"}";

      send(postData, token);
      // sendTest();

      // parar a leitura
      uid = "";
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      delay(2500);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("iconTAG");
    }
  }
}

void printolcd(int val) {
  if (val < 10) {
    lcd.print("0");
    lcd.print(val);
  }
  else {
    lcd.print(val);
  }
}

void timeGet() {
  Secs = second();
  Mins = minute();
  Hrs = hour();
}

void timeShow() {
  lcd.setCursor(4, 1);
  printolcd(Hrs);
  lcd.print(":");
  printolcd(Mins);
  lcd.print(":");
  printolcd(Secs);

  timeGet();

  Secs++;
  if (Secs == 60) {
    Secs = 0;
    Mins++;
  }
  if (Mins == 60)  {
    Mins = 0;
    Hrs++;
  }
  // if (Hrs == 13) {
  //   Hrs = 1;
  // }

  delay(900);
}
