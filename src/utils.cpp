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

Cursor::Cursor(int x, int y) {
    xPos = x;
    yPos = y;
}

void Cursor::setXPos(int x) {
    xPos = x;
}

void Cursor::setYPos(int y) {
    yPos = y;
}

int Cursor::getXPos() {
    return xPos;
}

int Cursor::getYPos() {
    return yPos;
}

void displayGame(MATRIX7219* display, Cursor* cursor, int movX, int movY) {
    int positions[8][8] = {0};
    int newX = cursor->getXPos() + movX;
    int newY = cursor->getYPos() + movY;
    if(newX < 1) {
        newX = 1;
    }else if(newX > 6) {
        newX = 6;
    }
    if(newY < 1) {
        newY = 1;
    }else if(newY > 6) {
        newY = 6;
    }
    cursor->setXPos(newX);
    cursor->setYPos(newY);
    display->clear();
    positions[cursor->getYPos()+1][cursor->getXPos()] = 1;
    positions[cursor->getYPos()-1][cursor->getXPos()] = 1;
    positions[cursor->getYPos()][cursor->getXPos()-1] = 1;
    positions[cursor->getYPos()][cursor->getXPos()+1] = 1;
    for(int i = 0; i < 8; i++) {
        int rowValue = 0;
        for(int j = 0; j < 8; j++) {
            rowValue |= (positions[i][j] << (7 - j));
        }
        display->setRow(i + 1, rowValue, 1);
    }
    /*display->setRow(cursor->getYPos(), 1 << (7 - cursor->getXPos()), 1);
    display->setRow(cursor->getYPos()+2, 1 << (7 - cursor->getXPos()), 1);
    display->setRow(cursor->getYPos()+1, 101 << (6 - cursor->getXPos()), 1);*/
}

MyTimer::MyTimer() {
    timeOfChange = 0;
}

bool MyTimer::hasTimePassed(unsigned long time) {
    if(this->timeOfChange == 0) this->timeOfChange = millis() + time;
    if(millis() >= this->timeOfChange) {
        this->timeOfChange = 0;
        return true;
    }else {
        return false;
    }
}