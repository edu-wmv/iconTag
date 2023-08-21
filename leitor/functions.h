#include "variables.h"

void(* resetFunc) (void) = 0;

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
String timeFix(int y, int mon, int day, int h, int min, int s) {
  char buffer[19];
  sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d", y, mon, day, h, min, s);
  return buffer;
}

void send() {
  if (client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println("Connected to server: ");

    client.println("GET /setPoint HTTP/1.1");
    client.println("Host: " + String(HOST) + ":" + String(HTTP_PORT));
    client.println("api-key: " + String(API_KEY));
    client.println("uuid: " + String(uid));
    client.println("time: " + String(time));
    client.println("Connection: close");
    client.println();

    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    client.flush();
    Serial.println();
    Serial.println("disconnected");
  } else {
    Serial.println("Connection failed :/");
  }
}

void tagReader() {
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    time = timeFix(year(), month(), day(), hour(), minute(), second());

    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {
      for ( int i = 0; i < rfid.uid.size; i++) {
        uid.concat(rfid.uid.uidByte[i]);
      }

      Serial.println(uid);
      //Serial.println("sending");
      //send();

      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      delay(2500);
      resetFunc();
    }
  }
}
