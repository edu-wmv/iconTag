#include <Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include "secrets.h"

// RFID MODULE PINS
#define RST_PIN   37 // RFID reset pin
#define SS_PIN    39 // RFID pin
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

String uid;
String fullTime;
String time;

EthernetClient client;

char data[200];
boolean isData = false;

LiquidCrystal_I2C lcd(0x27,16,2);

// TIME VARIABLES
int  Hrs;  
int  Mins;
int  Secs;
int  wday, d, m, yr;

unsigned long interval = 60UL * 60UL * 1000UL; // 1 hour
unsigned long lastTime = 0;