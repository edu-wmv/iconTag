#include <Ethernet.h>
#include <EthernetUdp.h>
#include <TimeLib.h>

#define RST_PIN   9
#define SS_PIN    8

byte mac[] = { 0xAA, 0xBB, 0xCC, 0x00, 0xFE, 0xED };

IPAddress timeServer(129,6,15,28);

const int timeZone = -3;

EthernetUDP Udp;
unsigned int localPort = 8888;

time_t prevDisplay = 0;

byte messageBuffer[48];