#include "functions.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  ethernetUDP();

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CADASTRO DE TAGS");
  lcd.setCursor(0, 1);
  lcd.print("----TERMINAL----");
  delay(2000);
  blueOn();
}

void loop() {
  uid = tagReader();
  Serial.println("Digite o nome: ");
  while(Serial.available() == 0) {}
  name = Serial.readString();

  Serial.println("\n========================");
  Serial.println("\nVerifique os dados:");
  Serial.print("Nome: ");
  Serial.println(name);
  Serial.print("UID: ");
  Serial.println(uid);

  Serial.println("\nOs dados estao corretos?");
  Serial.println("1 - SIM \n2 - NAO");
  while(Serial.available() == 0) {}
  check = Serial.parseInt();

  switch(check) {
    case 1:
      Serial.println("\nOK");
      sendData();
      break;
    case 2:
      Serial.print("\nOperação cancelada.");
      break;
    default:
      Serial.print("\nSelecione uma opção válida");
      break;
  }

  while(1);
} 