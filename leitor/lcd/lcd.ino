#include "functions.h"

void setup() {
  Serial.begin(9600);		                         // inicia o comunicação do código com o serial
  SPI.begin();                                   // inicia o spi
  rfid.PCD_Init();                               // inicia o leitor de tag63

  ethernetUDP();
  setSyncProvider(getNtpTime);

	Serial.println("Aproxime a tag do leitor");
}

lcd.begin(16, 2);                                  //Inicia o lcd
  lcd.clear();
                                                  //Escreve no lcd
  lcd.print("BEM VINDO AO ICON!");

  lcd.setCursor(0, 1); 
  lcd.print("ACESSO REGISTRADO");
 


void loop() {
  tagReader();


for(int lcdpos = 0; lcdpos < 42; lcdpos ++)                // faz com que o que esta escrito va pra direita e esquerda
{
  lcd.setCursor(0,0);
  lcd.scrollDisplayRight();
  delay (250);
}

  for(int lcdpos = 0; lcdpos < 42; lcdpos ++)
{
lcd.setCursor(16,0);
lcd.scrollDisplayLeft();
delay (250);
}


void tagReader() {
  // tag disponível
  if (rfid.PICC_IsNewCardPresent()) {
    String time = timeFix(hour(), minute(), second());
    String uid;
    // aqui a tag foi lida já         
    if (rfid.PICC_ReadCardSerial()) {
      // esse código cria a variavél para armazenar as infos do tipo da tag
      MFRC522::PICC_Type picc_Type = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(picc_Type));

      // retorna UID da tag em HEX
      Serial.print("UID: ");
      for ( int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : ":"); // FIX
        Serial.print(rfid.uid.uidByte[i], HEX);
        uid.concat(rfid.uid.uidByte[i]);
      }
      
      Serial.print("\nTime: ");
      Serial.println(time);
      Serial.print("UID Geral: ");
      Serial.println(uid);



      // parar a leitura
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }else{
    lcd.noDisplay();
  }
}
