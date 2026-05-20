#include <Arduino.h>
#include <MATRIX7219.h>
#include <ESP32RotaryEncoder.h>
#include <Joystick.hpp>
#include "utils.hpp"

Cursor cursor;
MATRIX7219 display(DATA_PIN, SELECT_PIN, CLOCK_PIN, NUM_DEVICES);
RotaryEncoder encoder(ENCODE_DATA_PIN, ENCODE_CLOCK_PIN);
Joystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);
Projectile projectile(0,3);
MyTimer timer;
void displaySetup();
void encoderSetup();

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  displaySetup();
  encoderSetup();
  testDisplay(&display);
}

void loop() {
  joystick.update();
  //Serial.printf("X: %d, Y: %d, SW: %d\n", joystick.getX(), joystick.getY(), joystick.getSW());
  displayGame(&display, &cursor, &projectile, &timer, joystick.getX(), joystick.getY(), encoder.getEncoderValue(), joystick.getSW());
  if(digitalRead(BUTTON_PIN)==0){
    digitalWrite(BUZZ_PIN, 1);
  }else{
    digitalWrite(BUZZ_PIN, 0);
  }
  delay(25);
}

void displaySetup() {
  display.begin();
  display.clear();
  display.displayOn();
  //display.setSwap(true);
  display.setBrightness(1);
}

void encoderSetup() {
  encoder.setEncoderType(EncoderType::HAS_PULLUP);
  encoder.setBoundaries(1, 10, true);
  encoder.onTurned( [](long value) {
    Serial.printf("Value: %ld\n", value);
  });
  encoder.begin();
};