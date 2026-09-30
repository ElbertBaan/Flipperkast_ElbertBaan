#include <Servo.h>

Servo motorlinks;
Servo motorrechts;

const int knopLinks = 11;
const int knopRechts = 10;

const int irSensor = 12;
const int ledPin = 13;

void setup() {
  pinMode(knopLinks, INPUT_PULLUP);
  pinMode(knopRechts, INPUT_PULLUP);

  pinMode(irSensor, INPUT);
  pinMode(ledPin, OUTPUT);

  motorlinks.attach(9);
  motorrechts.attach(8);

  motorlinks.write(0);
  motorrechts.write(95);
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

  // IR sensor
  if (digitalRead(irSensor) == LOW) {
    digitalWrite(ledPin, HIGH); // LED aan
  } else {
    digitalWrite(ledPin, LOW);  // LED uit
  }

  delay(10);
}