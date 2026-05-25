#include <Arduino.h>
#include <MATRIX7219.h>
#include <ESP32RotaryEncoder.h>
#include <Joystick.hpp>
#include "utils.hpp"

Cursor cursor;
MATRIX7219 display(DATA_PIN, SELECT_PIN, CLOCK_PIN, NUM_DEVICES);
RotaryEncoder encoder(ENCODE_DATA_PIN, ENCODE_CLOCK_PIN, ENCODE_SW_PIN);
Joystick joystick(JOYSTICK_X, JOYSTICK_Y, JOYSTICK_SW);
Projectile projectile(0,3);
MyTimer timer;
MyTimer mainTimer;

const unsigned long GAME_DURATION_MS = 90000UL; // 15 seconds for testing; change to 120000UL for 2 minutes
long level = 1;
bool gameRunning = false;

void displaySetup();
void encoderSetup();

unsigned long points = 0;

void setup() {
  Serial.begin(9600);
  pinMode(JOYSTICK_SW, INPUT_PULLUP);
  pinMode(ENCODE_SW_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  displaySetup();
  encoderSetup();
  testDisplay(&display);
  projectile.RandStart();
}

void loop() {
  joystick.update();
  
  if(!gameRunning){
    level = encoder.getEncoderValue();
    //Serial.printf("%ld\n", level);
  }

  if(gameRunning){
    displayGame(&display, &cursor, &projectile, &timer, joystick.getX(), joystick.getY(), &points, level, joystick.getSW());
    if(digitalRead(JOYSTICK_SW) == 0){
      digitalWrite(BUZZ_PIN, HIGH);
    } else {
      digitalWrite(BUZZ_PIN, LOW);
    }

    if(mainTimer.hasTimePassed(GAME_DURATION_MS)){
      digitalWrite(BUZZ_PIN, LOW);
      resetDisplay(&display);
      gameRunning = false;
      Serial.println("\033[2J");
      Serial.println("SKONCIL JSI!");
      Serial.printf("BODY: %lu\n", points);
      points = 0;
    }
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
    static unsigned long lastTurn = 0;
    const unsigned long DEBOUNCE_MS = 5;
    unsigned long now = millis();
    if (now - lastTurn < DEBOUNCE_MS) return;
    lastTurn = now;
    Serial.println("\033[2J");
    Serial.printf("Current difficulty: %ld\n", value);
  });
  encoder.onPressed([](unsigned long duration){
    //Serial.println("\033[2J");
    //Serial.printf("Pressed :3\n");
    static unsigned long lastPress = 0;
    const unsigned long DEBOUNCE_MS = 100;
    unsigned long now = millis();
    if (now - lastPress < DEBOUNCE_MS) return;
    lastPress = now;
    if (duration >= 200) {
      gameRunning = true;
    }
  });
  encoder.begin();
};