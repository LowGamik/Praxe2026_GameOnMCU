#include <Arduino.h>
#include <MATRIX7219.h>
#include <Joystick.hpp>
#include "utils.hpp"

MATRIX7219 display(DATA_PIN, SELECT_PIN, CLOCK_PIN, NUM_DEVICES);
void displaySetup();

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  displaySetup();
  testDisplay(&display);
}

void loop() {
  if(digitalRead(BUTTON_PIN)==0){
    digitalWrite(BUZZ_PIN, 1);
  }else{
    digitalWrite(BUZZ_PIN, 0);
  }
}

void displaySetup() {
  display.begin();
  display.clear();
  display.displayOn();
  //display.setSwap(true);
  display.setBrightness(1);
}