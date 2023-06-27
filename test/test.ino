#include "functions.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Aproxime a tag do leitor: ");
}

void loop() {
  tagReader();
}