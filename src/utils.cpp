#include <Arduino.h>
#include <MATRIX7219.h>
#include "utils.hpp"

void testDisplay(MATRIX7219* display) {
    for(int i = 0; i < 8; i++) {
        display->setRow(i + 1, 0b11111111, 1);
        delay(200);
    }
    display->clear();
}