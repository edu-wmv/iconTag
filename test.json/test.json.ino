#include <SPI.h>
#include <ArduinoJson.h>

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
while (!Serial) continue;

DynamicJsonDocument doc(1024);
  doc["id"] = 42;  // devolve o o uid da tag
  doc["horA"] = 43; // devolve a hora
  serializeJson(doc, Serial);
  
}
void loop() {
  // put your main code here, to run repeatedly:

}
