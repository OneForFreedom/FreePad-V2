#include "Keyboard.h"
#include "HID-Project.h" // For media keys

// ====== Key Matrix ======
const int ROWS = 3;
const int COLS = 5;
const int rowPins[ROWS] = {6, 7, 0};
const int colPins[COLS] = {26, 27, 28, 29, 2};

char keymap[ROWS][COLS] = {
  {'1','2','3','4','5'},
  {'6','7','8','9','0'},
  {'q','w','e','r','t'}
};

// ====== Rotary Encoder ======
const int encA = 1;
const int encB = 2;
int lastEncState = 0;

void setup() {
  Keyboard.begin();
  Consumer.begin(); // For media keys

  // Matrix setup
  for (int r=0; r<ROWS; r++) pinMode(rowPins[r], INPUT_PULLUP);
  for (int c=0; c<COLS; c++) { pinMode(colPins[c], OUTPUT); digitalWrite(colPins[c], HIGH); }

  // Encoder pins
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);

  lastEncState = digitalRead(encA);
}

void loop() {
  // ====== Scan key matrix ======
  for (int c=0; c<COLS; c++) {
    digitalWrite(colPins[c], LOW);
    for (int r=0; r<ROWS; r++) {
      if (digitalRead(rowPins[r]) == LOW) Keyboard.press(keymap[r][c]);
      else Keyboard.release(keymap[r][c]);
    }
    digitalWrite(colPins[c], HIGH);
  }

  // ====== Read rotary encoder ======
  int encState = digitalRead(encA);
  if (encState != lastEncState) {
    if (digitalRead(encB) != encState) {
      Consumer.write(MEDIA_VOLUME_UP);   // Clockwise
    } else {
      Consumer.write(MEDIA_VOLUME_DOWN); // Counterclockwise
    }
  }
  lastEncState = encState;
}
