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

void displayGame(MATRIX7219* display, Cursor* cursor, Projectile* projectile, MyTimer *timer, int movX, int movY, unsigned long *pointPTR, int level, int switchState) {
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
    
    if(timer->hasTimePassed(500/level)) {
        projectile->move();
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
            //Serial.println("\033[2J"); // Clear console
            //Serial.println("Hit!");
            (*pointPTR)++;
            //Serial.printf("Current points: %lu\n", *pointPTR);
            projectile->RandStart();
        }
    }
}

MyTimer::MyTimer() {
    timeOfChange = 0;
}

bool MyTimer::hasTimePassed(unsigned long time) {
    if(this->timeOfChange == 0) this->timeOfChange = millis() + time;
    if(millis() < this->timeOfChange) {
        return false;
    }else {
        this->timeOfChange = 0;
        return true;
    }
}

Projectile::Projectile(int x, int y) {
    xPos = x;
    yPos = y;
}

int Projectile::getXPos() {
    return xPos;
}

int Projectile::getYPos() {
    return yPos;
}

void Projectile::move() {
    xPos += xDir;
    yPos += yDir;

    if(DirState::UP == dir) {
        if(yPos < 0) {
            yPos = 0;
            dir = DirState::DOWN;
            yDir = -yDir;
        }
    } else if(DirState::DOWN == dir) {
        if(yPos > 7) {
            yPos = 7;
            dir = DirState::UP;
            yDir = -yDir;
        }
    } else if(DirState::LEFT == dir) {
        if(xPos < 0) {
            xPos = 0;
            dir = DirState::RIGHT;
            xDir = -xDir;
        }
    } else if(DirState::RIGHT == dir) {
        if(xPos > 7) {
            xPos = 7;
            dir = DirState::LEFT;
            xDir = -xDir;
        }
    }
}

void Projectile::setXPos(int xDir) {
    this->xPos = xDir;
}

void Projectile::setYPos(int yDir) {
    this->yPos = yDir;
}

void Projectile::RandStart() {
    this->dir = intToDirState(random(0, 4));
    switch(this->dir) {
        case UP:
            xPos = random(1, 6);
            yPos = 0;
            xDir = 0;
            yDir = -1;
            break;
        case DOWN:
            xPos = random(1, 6);
            yPos = 7;
            xDir = 0;
            yDir = 1;
            break;
        case LEFT:
            xPos = 7;
            yPos = random(1, 6);
            xDir = -1;
            yDir = 0;
            break;
        case RIGHT:
            xPos = 0;
            yPos = random(1, 6);
            xDir = 1;
            yDir = 0;
            break;
        default:
            xPos = random(1, 6);
            yPos = random(1, 6);
            xDir = 0;
            yDir = 0;
    }
}

DirState intToDirState(int number){
    switch(number){
        case 0: return UP;
        case 1: return DOWN;
        case 2: return LEFT;
        case 3: return RIGHT;
        default: return NONE;
    }
}

void resetDisplay(MATRIX7219* display){
    


    for(int i = 1; i <= 8; i++){
        display->setRow(i,0,1);
    }

    for(int i = 1; i <= 8; i++){
        display->setRow(i,255,1);
        delay(100);
    }

    for(int i = 1; i <= 8; i++){
        display->setRow(i,0,1);
        delay(100);
    }
}

void testBuzzer(){
    digitalWrite(BUZZ_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZ_PIN, LOW);
}