#include <Servo.h>

Servo motorlinks;
Servo motorrechts;

const int knopLinks = 11;
const int knopRechts = 10;

void setup() {
  pinMode(knopLinks, INPUT_PULLUP);
  pinMode(knopRechts, INPUT_PULLUP);

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
    motorrechts.write(95);
  } else {
    motorrechts.write(0);
  }

  delay(10);
}