#include "variables.h"


void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Iniciar cadastro...");
}

String uid;

void loop() {
  uid = tagReader();
  Serial.println(uid);

  while(1);
}