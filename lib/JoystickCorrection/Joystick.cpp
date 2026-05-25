#include <Arduino.h>
#include "Joystick.hpp"

/**
 * Konstruktor třídy Joystick - inicializuje joystick
 * @param pinX - pin pro osu X (analogový vstup)
 * @param pinY - pin pro osu Y (analogový vstup)
 * @param pinSW - pin pro tlačítko (digitální vstup)
 */
Joystick::Joystick(int pinX, int pinY, int pinSW) {
    xValue = 0; // Počáteční hodnota osy X
    yValue = 0; // Počáteční hodnota osy Y
    swValue = false; // Tlačítko není stisknuto
    this->pinX = pinX; // Ulož pin pro osu X
    this->pinY = pinY; // Ulož pin pro osu Y
    this->pinSW = pinSW; // Ulož pin pro tlačítko
    pinMode(pinX, INPUT); // Nastav pin X jako vstup
    pinMode(pinY, INPUT); // Nastav pin Y jako vstup
    pinMode(pinSW, INPUT_PULLUP); // Nastav pin tlačítka jako vstup s pull-up odporem
}

/**
 * Aktualizuj stav joysticku - přečti všechny vstupy
 */
void Joystick::update() {
    xValue = analogRead(pinX); // Přečti analogovou hodnotu osy X (0-4095)
    yValue = analogRead(pinY); // Přečti analogovou hodnotu osy Y (0-4095)
    swValue = !digitalRead(pinSW); // Přečti stav tlačítka (invertuj, protože pull-up = invertováno)
}

/**
 * Vrátí stav osy X joysticku
 * @return -1 (vlevo), 0 (střed), 1 (vpravo)
 */
int Joystick::getX() {
    if(xValue > 2500) { // Pokud je hodnota vysoká (joystick je tlačen vpravo)
        return -1; // Vrátí -1 (vlevo na displeji)
    }else if(xValue < 1500) { // Pokud je hodnota nízká (joystick je tlačen vlevo)
        return 1; // Vrátí 1 (vpravo na displeji)
    }else { // Pokud je hodnota v prostřední poloze
        return 0; // Vrátí 0 (bez pohybu)
    }
}

/**
 * Vrátí stav osy Y joysticku
 * @return -1 (nahoru), 0 (střed), 1 (dolů)
 */
int Joystick::getY() {
    if(yValue > 2500) { // Pokud je hodnota vysoká (joystick je tlačen dolů)
        return -1; // Vrátí -1 (nahoru na displeji)
    }else if(yValue < 1500) { // Pokud je hodnota nízká (joystick je tlačen nahoru)
        return 1; // Vrátí 1 (dolů na displeji)
    }else { // Pokud je hodnota v prostřední poloze
        return 0; // Vrátí 0 (bez pohybu)
    }
}

/**
 * Vrátí stav tlačítka joysticku
 * @return true pokud je tlačítko stisknuto, false jinak
 */
bool Joystick::getSW() {
    return swValue; // Vrátí stav tlačítka
}