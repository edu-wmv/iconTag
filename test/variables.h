#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>

// RFID MODULE
#define SS_PIN 49
#define RST_PIN 53
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD MODULE
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ETHERNET MODULE
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // end. mac
unsigned int localPort = 8888; // porta a ser usada no protcolo de rede
EthernetUDP Udp; // cria o protocolo iniciador do UDP
EthernetClient client;

// LED VARIABLES
int blue = 7;
int green = 8;
int red = 12;

// VARIABLES