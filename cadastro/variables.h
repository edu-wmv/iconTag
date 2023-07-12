#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

// RFID MODULE PINS
#define RST_PIN   9 
#define SS_PIN    8 
MFRC522 rfid(SS_PIN, RST_PIN);

// ETHERNET & TIME VARIABLES
byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED }; // end. mac
unsigned int localPort = 8888; // porta a ser usada no protcolo de rede
EthernetUDP Udp; // cria o protocolo iniciador do UDP
EthernetClient client;

int HTTP_PORT = 8080;
byte HOST_NAME[] = { 192, 168, 0, 123 };

String uid;
String name;
String query;
int check;

String API_KEY = "SUNPTl9pY29udGFn";
