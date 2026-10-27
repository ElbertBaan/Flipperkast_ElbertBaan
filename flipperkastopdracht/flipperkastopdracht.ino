#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Waardes
int levens = 3;
long score = 0;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Startknop
const int startKnop = 22;
bool spelGestart = false;

// Relais voor DC-motor
const int relaisPin = 2;

// Cooldowns
int lastScoreStatus = HIGH;
int lastLevensStatus = HIGH;

unsigned long lastScoreTime = 0;
unsigned long lastLevensTime = 0;

const unsigned long scoreCooldown = 500;
const unsigned long levensCooldown = 15000;

// Servo's
Servo motorlinks;
Servo motorrechts;

const int flipperlinks = 11;
const int flipperrechts = 10;

// IR-sensoren
const int irScorePin = 12;
const int irLevensPin = 7;

// LEDs
const int links = 51;
const int midden = 49;
const int rechts = 47;

unsigned long lastLedTime = 0;
bool ledStand = false;

void setup() {

  // Flippers
  pinMode(flipperlinks, INPUT_PULLUP);
  pinMode(flipperrechts, INPUT_PULLUP);

  motorlinks.attach(9);
  motorrechts.attach(8);

  motorlinks.write(0);
  motorrechts.write(0);

  // Startknop
  pinMode(startKnop, INPUT_PULLUP);

  // Relais
  pinMode(relaisPin, OUTPUT);
  digitalWrite(relaisPin, LOW);

  // IR-sensoren
  pinMode(irScorePin, INPUT);
  pinMode(irLevensPin, INPUT);

  // LEDs
  pinMode(links, OUTPUT);
  pinMode(midden, OUTPUT);
  pinMode(rechts, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Druk START");
  lcd.setCursor(0, 1);
  lcd.print("om te spelen");
}

void fliplinks() {
  if (digitalRead(flipperlinks) == LOW) {
    motorlinks.write(95);
  } else {
    motorlinks.write(0);
  }
}

void fliprechts() {
  if (digitalRead(flipperrechts) == LOW) {
    motorrechts.write(0);
  } else {
    motorrechts.write(95);
  }
}

void toonScore() {
  lcd.setCursor(0, 0);
  lcd.print("Score:");
  lcd.print(score);
  lcd.print("     ");
}

void toonLevens() {
  lcd.setCursor(0, 1);
  lcd.print("Levens:");
  lcd.print(levens);
  lcd.print("     ");
}

void ledverlichting() {

  unsigned long knipperSnelheid;

  if (levens == 3) {
    knipperSnelheid = 1000;
  }
  else if (levens == 2) {
    knipperSnelheid = 500;
  }
  else {
    knipperSnelheid = 150;
  }

  if (millis() - lastLedTime >= knipperSnelheid) {

    lastLedTime = millis();
    ledStand = !ledStand;

    if (ledStand) {
      digitalWrite(links, LOW);
      digitalWrite(rechts, LOW);
      digitalWrite(midden, HIGH);
    }
    else {
      digitalWrite(links, HIGH);
      digitalWrite(rechts, HIGH);
      digitalWrite(midden, LOW);
    }
  }
}

void gameOver() {

  digitalWrite(relaisPin, LOW);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("GAME OVER!");

  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.print(score);

  while (true) {
  }
}

void loop() {

  // Wachten op start
  if (!spelGestart) {

    if (digitalRead(startKnop) == LOW) {

      spelGestart = true;
      score = 0;
      levens = 3;

      digitalWrite(relaisPin, HIGH);

      lcd.clear();
      toonScore();
      toonLevens();
    }

    return;
  }

  // Flippers
  fliplinks();
  fliprechts();

  // LED effecten
  ledverlichting();

  // Score-sensor
  int scoreStatus = digitalRead(irScorePin);

  if (lastScoreStatus == HIGH && scoreStatus == LOW) {

    if (millis() - lastScoreTime >= scoreCooldown) {

      score += 750;

      toonScore();

      lastScoreTime = millis();
    }
  }

  lastScoreStatus = scoreStatus;

  // Levens-sensor
  int levensStatus = digitalRead(irLevensPin);

  if (lastLevensStatus == HIGH && levensStatus == LOW) {

    if (millis() - lastLevensTime >= levensCooldown) {

      levens--;

      toonLevens();

      lastLevensTime = millis();

      if (levens <= 0) {
        gameOver();
      }
    }
  }

  lastLevensStatus = levensStatus;
}