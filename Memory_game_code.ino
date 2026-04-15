#include <Arduino.h>

int ledPins[] = {2, 4, 7};
int buttonPins[] = {3, 5, 6};

int buzzer = 8;

int sequence[50];
int level = 0;

// -------- BUZZER FUNCTION --------
void beep(int freq, int duration) {
  tone(buzzer, freq, duration);
  delay(duration);
  noTone(buzzer);
}

// -------- SETUP --------
void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(0));
}

// -------- MAIN LOOP --------
void loop() {
  addToSequence();
  playSequence();

  if (checkUserInput()) {
    level++;
    beep(1000, 200); // correct sound
    delay(500);
  } else {
    gameOver();
    level = 0;
    delay(1500);
  }
}

// -------- ADD STEP --------
void addToSequence() {
  sequence[level] = random(0, 3);
}

// -------- SHOW SEQUENCE --------
void playSequence() {
  for (int i = 0; i <= level; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH);

    beep(500 + sequence[i] * 200, 200);

    delay(200);
    digitalWrite(ledPins[sequence[i]], LOW);
    delay(300);
  }
}

// -------- USER INPUT --------
bool checkUserInput() {
  for (int i = 0; i <= level; i++) {
    int input = waitForButton();
    if (input != sequence[i]) {
      return false;
    }
  }
  return true;
}

// -------- BUTTON READ --------
int waitForButton() {
  while (true) {
    for (int i = 0; i < 3; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {

        // small delay to stabilize
        delay(30);

        // confirm still pressed
        if (digitalRead(buttonPins[i]) == LOW) {

          // show feedback (LED blink)
          digitalWrite(ledPins[i], HIGH);
          beep(800, 100);
          delay(150);
          digitalWrite(ledPins[i], LOW);

          // wait until released
          while (digitalRead(buttonPins[i]) == LOW);

          delay(100); // gap before next input

          return i;
        }
      }
    }
  }
}

// -------- GAME OVER --------
void gameOver() {
  for (int t = 0; t < 5; t++) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPins[i], HIGH);
    }

    beep(200, 300);

    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPins[i], LOW);
    }

    delay(200);
  }
}