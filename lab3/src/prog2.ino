#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;
int pos = 0;
bool state = true; // true - zamkniete drzwi

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(0);
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  while (Serial.available() == 0) {}
  String op = Serial.readString();
  op.trim();
  if (op == "o" && state) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Drzwi sie");
      lcd.setCursor(0,1);
      lcd.print("Otwieraja");
      state = false;
      for(pos = 0; pos <= 180; pos += 1) // obrót od 0 do 180 stopni
      {
        myservo.write(pos);
        delay(15);
      }
      lcd.clear();
  } else if (op == "z" && !state) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Drzwi sie");
      lcd.setCursor(0,1);
      lcd.print("Zamykaja");
      state = true;
      for(pos = 180; pos>=0; pos-=1) // obrót od 180 do 0 stopni
      {
        myservo.write(pos);
        delay(15);
      }
      lcd.clear();
  } else if (op == "z" && state){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Drzwi juz sa");
      lcd.setCursor(0,1);
      lcd.print("Zamkniete");
  } else if (op == "o" && !state){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Drzwi juz sa");
      lcd.setCursor(0,1);
      lcd.print("Otwarte");
      delay(15*180);
  } else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Niepoprawna");
      lcd.setCursor(0,1);
      lcd.print("Operacja");
      delay(15*180);
  }
}
