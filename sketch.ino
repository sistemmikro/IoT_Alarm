#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin setup
const int pirPin = 7;
const int buzzerPin = 5;
const int buttonPin = 2;
const int servoPin = 6;

// create instance servo 
Servo myServo;

// State
bool systemActive = false;
bool motionTriggered = false;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 

  myServo.attach(servoPin);
  Gate(1); 

  lcd.begin(16, 2);
  lcd.backlight();

  setNotifikasi("Tekan tombol",0,0,true);
  setNotifikasi("untuk aktifkan",0,1,false);

}

void loop() {
   if (digitalRead(buttonPin) == LOW) {
    delay(200);
     if (!systemActive) {
      setSystem(true);
    } else {
      setSystem(false);
    }
    delay(1000);
   }
   
  if (systemActive && !motionTriggered && digitalRead(pirPin) == HIGH) {

    motionTriggered = true;
    setNotifikasi("Terdeteksi !!",0,0,true);
    setNotifikasi("Ruangan terkunci",0,1,false);
    Gate(0);
    AlarmOn();
  }
}



void setSystem(bool isActive){
  if(isActive){
      systemActive = true;
      motionTriggered = false;
      Gate(1);
      setNotifikasi("Sensor aktif",0,0,true);
  }else{
      systemActive = false;
      motionTriggered = false;
      Gate(1);
      setNotifikasi("Reset",0,0,true);
      delay(1000);
      setNotifikasi("Tekan Tombol",0,0,true);
      setNotifikasi("Untuk Aktifkan",0,1,false);
  }
}

void AlarmOn() {
    
    while (digitalRead(buttonPin) == HIGH) { 
    tone(buzzerPin, 1000);
    delay(200);
    tone(buzzerPin, 1500);
    delay(200);
  }
  noTone(buzzerPin);
  setSystem(false);
  delay(500);
}

void Gate(int i){
  if(i == 1){
    myServo.write(90);
  }else{
    myServo.write(0);
  }
}

void setNotifikasi(String s,int c,int r,bool ce){
   if(ce == true) {lcd.clear();}
    lcd.setCursor(c, r);
    lcd.print(s);
}

