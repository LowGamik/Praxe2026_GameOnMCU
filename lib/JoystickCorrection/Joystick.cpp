#include <Arduino.h>
#include "Joystick.hpp"

Joystick::Joystick(int pinX, int pinY, int pinSW) {
    xValue = 0;
    yValue = 0;
    swValue = false;
    this->pinX = pinX;
    this->pinY = pinY;
    this->pinSW = pinSW;
    pinMode(pinX, INPUT);
    pinMode(pinY, INPUT);
    pinMode(pinSW, INPUT_PULLUP);
}

void Joystick::update() {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    swValue = !digitalRead(pinSW);
}

int Joystick::getX() {
    return xValue;
}

int Joystick::getY() {
    return yValue;
}

bool Joystick::getSW() {
    return swValue;
}