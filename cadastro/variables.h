#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include "secrets.h"

// RFID MODULE PINS
#define RST_PIN   37 
#define SS_PIN    39 
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD MODULE
LiquidCrystal_I2C lcd(0x27,16,2);

// ETHERNET & TIME VARIABLES
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED }; // end. mac
unsigned int localPort = 8888; // porta a ser usada no protcolo de rede
EthernetUDP Udp; // cria o protocolo iniciador do UDP
EthernetClient client;

String uid;
String name;
String query;
String user_id;

bool isData = false;
char data[200];