#include <Servo.h>
#include <LiquidCrystal.h>

Servo motorlinks;
Servo motorrechts;

// LCD pins: RS,E,D4,D5,D6,D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int knopLinks = 11;
const int knopRechts = 10;

const int irSensor = 12;
const int ledPin = 13;

int score = 0;
int vorigeStatus = HIGH;

void setup() {
  pinMode(knopLinks, INPUT_PULLUP);
  pinMode(knopRechts, INPUT_PULLUP);

  pinMode(irSensor, INPUT);
  pinMode(ledPin, OUTPUT);

  motorlinks.attach(9);
  motorrechts.attach(8);

  motorlinks.write(0);
  motorrechts.write(95);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score:");
  lcd.setCursor(0, 1);
  lcd.print(score);
}

void loop() {

  // Linker flipper
  if (digitalRead(knopLinks) == LOW) {
    motorlinks.write(95);
  } else {
    motorlinks.write(0);
  }

  // Rechter flipper
  if (digitalRead(knopRechts) == LOW) {
    motorrechts.write(0);
  } else {
    motorrechts.write(95);
  }

  // IR-sensor
  int sensorStatus = digitalRead(irSensor);

  if (vorigeStatus == HIGH && sensorStatus == LOW) {
    score++;

    lcd.setCursor(0, 1);
    lcd.print("                "); // regel wissen
    lcd.setCursor(0, 1);
    lcd.print(score);

    digitalWrite(ledPin, HIGH);
  }

  if (sensorStatus == HIGH) {
    digitalWrite(ledPin, LOW);
  }

  vorigeStatus = sensorStatus;
}