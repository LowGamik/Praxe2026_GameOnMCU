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

void displayGame(MATRIX7219* display, Cursor* cursor, Projectile* projectile, MyTimer *timer, int movX, int movY, int level, int switchState) {
    static MyTimer moveTimer;
    int positions[8][8] = {0};
    static int newX = 0;
    static int newY = 0;

    if(movX != 0 || movY != 0) {
        if (moveTimer.hasTimePassed(100))
        {
            newX = cursor->getXPos() + movX;
            newY = cursor->getYPos() + movY;
        }
    }

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
    
    if(timer->hasTimePassed(1000/level)) {
        projectile->move();
    }

    if(projectile->getXPos() >= 8) {
        projectile->setXPos(0);
    }
    positions[projectile->getYPos()][projectile->getXPos()] = 1;

    cursor->setXPos(newX);
    cursor->setYPos(newY);
    display->clear();
    positions[cursor->getYPos()+1][cursor->getXPos()] = !positions[cursor->getYPos()+1][cursor->getXPos()];
    positions[cursor->getYPos()-1][cursor->getXPos()] = !positions[cursor->getYPos()-1][cursor->getXPos()];
    positions[cursor->getYPos()][cursor->getXPos()-1] = !positions[cursor->getYPos()][cursor->getXPos()-1];
    positions[cursor->getYPos()][cursor->getXPos()+1] = !positions[cursor->getYPos()][cursor->getXPos()+1];
    

    for(int i = 0; i < 8; i++) {
        int rowValue = 0;
        for(int j = 0; j < 8; j++) {
            rowValue |= (positions[i][j] << (7 - j));
        }
        display->setRow(i + 1, rowValue, 1);
    }

    if(switchState == 1) {
       if(cursor->getXPos() == projectile->getXPos() && cursor->getYPos() == projectile->getYPos()) {
            Serial.println("Hit!");
        }
    }
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

Projectile::Projectile(int x, int y/*DirState dir*/) {
    xPos = x;
    yPos = y;
    //this->dir = dir;
}

int Projectile::getXPos() {
    return xPos;
}

int Projectile::getYPos() {
    return yPos;
}

void Projectile::move() {
    xPos += xDir;
    //yPos += yDir;
}

void Projectile::setXPos(int xDir) {
    this->xPos = xDir;
}

