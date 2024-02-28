#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

int Sms = 0;
int Detector = 7;

SoftwareSerial mySerial(8, 9);
LiquidCrystal_I2C lcd(0x3F, 20, 4);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(Detector, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("SYSTEM");
  lcd.setCursor(3, 2);
  lcd.print("INITIALIZATION");
  delay(2000);

}

void loop() {
  int Results = digitalRead(A1);
  Serial.println(Results);
  if(Results == 1 && Sms == 0){
    SendMessage();
    Sms = 1;
  }else if (Results == 0){
    Sms = 0;
  }
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("WEAPON DETECTOR");
  lcd.setCursor(0, 1);
  if(Results == 0){
    lcd.print("NO WEAPON DETECTED");
  }else if(Results == 1){
    lcd.print("WEAPON DETECTED");
  }
  lcd.setCursor(1, 2);
  lcd.print("SEND NOTIFICATION ");
  lcd.setCursor(0,3);
  if(Sms == 0){
    lcd.print("NO ALERT!!");
  }else{
    lcd.print("SENDING MESSAGE...");
  }
  delay(1000);

}
void SendMessage() {
  mySerial.println("AT");  //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1");  // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+254720972765\"");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("ALERT!! WEAPON DETECTED");  //text content
  updateSerial();
  mySerial.write(26);
  delay(1000);
}
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  //Forward what Software Serial received to Serial Port
  }
}
