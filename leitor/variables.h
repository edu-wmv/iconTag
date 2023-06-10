#include <Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SD.h>

// RFID MODULE PINS
#define RST_PIN   9
#define SS_PIN    8
MFRC522 rfid(SS_PIN, RST_PIN); // rfid initializator

// ETHERNET & TIME VARIABLES
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED };
IPAddress timeServer(129,6,15,28);
const int timeZone = -3;
EthernetUDP Udp;
unsigned int localPort = 8888;
time_t prevDisplay = 0;
byte messageBuffer[48];
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// SD CARD VARIABLES
File tagFile;