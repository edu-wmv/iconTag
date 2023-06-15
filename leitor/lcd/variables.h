#include <Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h

// RFID MODULE PINS
#define RST_PIN   9 // RFID reset pin
#define SS_PIN    8 // RFID pin
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

// ETHERNET & TIME VARIABLES
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED }; // end. mac
IPAddress timeServer(129,6,15,28); // server de request ntp
const int timeZone = -3; // fuso-horaŕio
EthernetUDP Udp; // cria o protocolo iniciador do UDP
unsigned int localPort = 8888; // porta a ser usada no protcolo de rede
time_t prevDisplay = 0;
byte messageBuffer[48];
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

const int rs = 33, en = 31, d4 = 22, d5 = 24, d6 = 40, d7 = 50;   //portas do lcd
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
