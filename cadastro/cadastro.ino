#include "functions.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  ethernetUDP();

  lcd.clear();
  lcd.setCursor(0, 0);
  printOnCenter("iconTAG");
  lcd.setCursor(0, 1);
  printOnCenter("SIGA O SERIAL");
  delay(2000);
}

void loop() {
  Serial.println("\niconTAG");

  do {
    lcd.clear();
    lcd.setCursor(0, 0);
    printOnCenter("Aproxime a tag");
    uid = tagReader();
  } while (uid.length() == 0);
  lcd.clear();
  lcd.setCursor(0, 0);
  printOnCenter("iconTAG");
  Serial.println("Nome iconico: ");
  while(Serial.available() == 0) {}
  name = Serial.readString();
  registerNewUser();
  delay(7000);
}

// IMPLEMENTAR FUNÇÃO DE ATUALIZAR USUÁRIO
// void loop() {
//   Serial.println("\niconTAG");
//   Serial.println("\nNovo cadastro - 1\nAtualizar usuario - 2");
//   while(Serial.available() == 0) {}
//   String userExists = Serial.readString();

//   if (userExists == "1") {
//     do {
//       lcd.clear();
//       lcd.setCursor(0, 0);
//       printOnCenter("Aproxime a tag");
//       uid = tagReader();
//     } while (uid.length() == 0);
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     printOnCenter("iconTAG");
//     Serial.println("Nome iconico: ");
//     while(Serial.available() == 0) {}
//     name = Serial.readString();
//     registerNewUser();
//   } else if (userExists == "2") {
//     Serial.println("Digite o ID do usuário: ");
//     while(Serial.available() == 0) {}
//     user_id = Serial.readString();
//     updateUser();
//   } else {
//     Serial.println("Opção inválida");
//   }
// }