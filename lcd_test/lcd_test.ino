#include "functions.h"

const int a_sec = 900; // clock second hand

int  Hrs; 
int  Mins;
int  Secs; 
String AmPm;  // to store the string data to display  'AM' or 'PM'
int ampm;     // to store user input
int wday, d, m, yr;
char const *DAYS[] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
char const *MONTHS[] = {"Jan", "Fev", "Mar", "Abr", "Maio", "Junho",
                        "Julho",  "Ago", "Set", "Out", "Nov", "Dez"};

void setup()
{
  Serial.begin(9600);
  ethernetUDP();
  setSyncProvider(getNtpTime);
  lcd.init();
  lcd.backlight();
  clkInit();
}

void loop()
{
  //...Display Time to LCD...
 lcd.setCursor(0, 1);
  lcd.print("Time:  ");
  printolcd(Hrs);
  lcd.print(":");
  printolcd(Mins);
  lcd.print(":");
  printolcd(Secs);
  lcd.print(AmPm);

  //...Clock starts...
  Secs = second();
  Secs++;
  if (Secs == 60) {
    Secs = 0;
    Mins++;
  }
  Mins = minute();
  if (Mins == 60)  {
    Mins = 0;
    Hrs++;
  }
  Hrs = hour();
  if (Hrs == 13) {
    Hrs = 1;
  }
  if (Hrs == 12 && Mins == 00 && Secs == 00) {
    if (AmPm == "AM")  {
      AmPm = "PM";
    }
    else {
      AmPm = "AM";
      d++;
      reset_dom(m); // reset the day to 1 when the days of the month exceed it's  limit

      //...Reset LCD row 0 and update date...
      lcd.setCursor(0,  0);
      lcd.print("                    ");
      lcd.setCursor(0, 0);
      lcd.print(DAYS[wday]);   // mon, tues, wed... etc.
      lcd.print(", ");
      printolcd(d);
      lcd.print(" ");
      lcd.print(MONTHS[m - 1]);
      lcd.print(" ");
      lcd.print(yr);
      wday++;
    }
  }
  delay(a_sec);  // delay acts as the clock second hand
}



//Initialise  the 'clock' settings////

void clkInit()  {

  d = Serial.parseInt();  // store day in integer variable  'd'
  d = day();

  m = Serial.parseInt();  // store month in integer variable 'm'
  m = month();

  yr = Serial.parseInt(); // store year in integer variable 'yr'
  yr = year();

  //... Display Date to LCD...
  lcd.setCursor(0, 0);
  printolcd(d);
  lcd.print(" ");
  lcd.print(MONTHS[m - 1]);
  lcd.print("  ");
  lcd.print(yr);



}

//To  put an extra '0' infront of the variables on the Serial Monitor////

void  printoserial(int val)
{
  if (val < 10) {
    Serial.print("0");
    Serial.print(val);
  }
  else {
    Serial.print(val);
  }
}

//To  put an extra '0' infront of the variables on the LCD

void  printolcd(int val)
{
  if (val < 10) {
    lcd.print("0");
    lcd.print(val);
  }
  else {
    lcd.print(val);
  }
}

void reset_dom(int var)  {
  switch (var) {
    case 1:   // Jan - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 2:   // Feb  - 28 days
      if (d == 29) {
        d = 1;
        m++;
      }
      break;
    case 3:   // March - 31 days
      if (d == 32) {
        d  = 1;
        m++;
      }
      break;
    case 4:   // April - 30 days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 5:   // May - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 6:   // June - 30 days
      if (d == 31)  {
        d = 1;
        m++;
      }
      break;
    case 7:   //  July - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 8:   // August - 31 days
      if (d == 32) {
        d  = 1;
        m++;
      }
      break;
    case 9:   // September -  30 days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 10:  // October - 31 days
      if (d == 32) {
        d  = 1;
        m++;
      }
      break;
    case 11:  // November - 30  days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 12:  // December - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
  }
}
