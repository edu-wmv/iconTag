#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

// RFID MODULE PINS
#define RST_PIN   37 
#define SS_PIN    39 
MFRC522 rfid(SS_PIN, RST_PIN);

// LCD MODULE
//(RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(33, 31, 28, 26 ,24, 22);

// LED VARIABLES
int blue = 5;
int green = 6;
int red = 7;

// ETHERNET & TIME VARIABLES
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED }; // end. mac
unsigned int localPort = 8888; // porta a ser usada no protcolo de rede
EthernetUDP Udp; // cria o protocolo iniciador do UDP
EthernetClient client;

int HTTP_PORT = 8080;
byte HOST_NAME[] = { 192, 168, 0, 109 };
String HOST = "192.168.0.109";

String uid;
String name;
String query;
int check;

String API_KEY = "SUNPTl9pY29udGFn";
