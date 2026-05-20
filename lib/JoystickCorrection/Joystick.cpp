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
    if(xValue > 2500) {
        return -1;
    }else if(xValue < 1500) {
        return 1;
    }else {
        return 0;
    }
}

int Joystick::getY() {
    if(yValue > 2500) {
        return -1;
    }else if(yValue < 1500) {
        return 1;
    }else {
        return 0;
    }
}

bool Joystick::getSW() {
    return swValue;
}