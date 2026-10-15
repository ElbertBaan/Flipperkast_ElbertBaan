#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensoren
const int scoreSensor = 12;
const int levensSensor = 7;

// Relais
const int relaisPin = 2;

// Spelgegevens
long score = 0;
int levens = 3;

// Sensorstatus
int vorigeScoreStatus = HIGH;
int vorigeLevensStatus = HIGH;

// Cooldown voor levenssensor
unsigned long laatsteLevensDetectie = 0;
const unsigned long levensCooldown = 15000; // 15 seconden

void setup() {
  pinMode(scoreSensor, INPUT);
  pinMode(levensSensor, INPUT);

  pinMode(relaisPin, OUTPUT);

  // Motor aan via relais
  digitalWrite(relaisPin, HIGH);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Score: 0");

  lcd.setCursor(0, 1);
  lcd.print("Levens: 3");
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("Score:       ");
  lcd.setCursor(7, 0);
  lcd.print(score);

  lcd.setCursor(0, 1);
  lcd.print("Levens:      ");
  lcd.setCursor(8, 1);
  lcd.print(levens);
}

void gameOver() {
  digitalWrite(relaisPin, LOW); // motor uit

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAME OVER");

  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.print(score);

  while (true) {
  }
}

void loop() {

  // SCORE SENSOR (+1000)
  int scoreStatus = digitalRead(scoreSensor);

  if (vorigeScoreStatus == HIGH && scoreStatus == LOW) {
    score += 1000;
    updateDisplay();
  }

  vorigeScoreStatus = scoreStatus;

  // LEVENS SENSOR (-1 leven)
  int levensStatus = digitalRead(levensSensor);

  if (vorigeLevensStatus == HIGH && levensStatus == LOW) {

    if (millis() - laatsteLevensDetectie >= levensCooldown) {

      levens--;

      if (levens < 0) {
        levens = 0;
      }

      updateDisplay();

      laatsteLevensDetectie = millis();

      if (levens == 0) {
        gameOver();
      }
    }
  }

  vorigeLevensStatus = levensStatus;
}