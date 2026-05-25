#include <Arduino.h>
#include <MATRIX7219.h>
#include "utils.hpp"

/**
 * Test funkcí displeje - postupně rozsvěcuje jednotlivé řádky matice
 * @param display - ukazatel na objekt LED matice
 */
void testDisplay(MATRIX7219* display) {
    for(int i = 0; i < 8; i++) { // Projdi všech 8 řádků matice
        display->setRow(i + 1, 0b11111111, 1); // Rozsvít celý řádek (0b11111111 = všechny LEDky zapnuty)
        delay(200); // Počkej 200ms pro vizuální efekt
    }
    display->clear(); // Zhasni všechny LEDky
}

/**
 * Konstruktor třídy Cursor - inicializuje pozici kurzoru
 * @param x - počáteční X pozice
 * @param y - počáteční Y pozice
 */
Cursor::Cursor(int x, int y) {
    xPos = x; // Ulož X pozici
    yPos = y; // Ulož Y pozici
}

/**
 * Nastavení X pozice kurzoru
 * @param x - nová X pozice
 */
void Cursor::setXPos(int x) {
    xPos = x; // Aktualizuj X pozici
}

/**
 * Nastavení Y pozice kurzoru
 * @param y - nová Y pozice
 */
void Cursor::setYPos(int y) {
    yPos = y; // Aktualizuj Y pozici
}

/**
 * Vrátí aktuální X pozici kurzoru
 * @return X pozice
 */
int Cursor::getXPos() {
    return xPos; // Vrátí X pozici
}

/**
 * Vrátí aktuální Y pozici kurzoru
 * @return Y pozice
 */
int Cursor::getYPos() {
    return yPos; // Vrátí Y pozici
}

/**
 * Hlavní herní funkce - aktualizuje a vykresluje herní scénu
 * @param display - ukazatel na LED matici
 * @param cursor - ukazatel na kurzor (hráč)
 * @param projectile - ukazatel na projektil (cíl)
 * @param timer - ukazatel na timer pro pohyb projektilu
 * @param movX - směr pohybu na ose X (-1, 0, 1)
 * @param movY - směr pohybu na ose Y (-1, 0, 1)
 * @param pointPTR - ukazatel na počet bodů
 * @param level - obtížnost hry (určuje rychlost projektilu)
 * @param switchState - stav tlačítka joysticku
 */
void displayGame(MATRIX7219* display, Cursor* cursor, Projectile* projectile, MyTimer *timer, int movX, int movY, unsigned long *pointPTR, int level, int switchState) {
    static MyTimer moveTimer; // Timer pro kontrolu rychlosti pohybu kurzoru
    int positions[8][8] = {0}; // Matice pro uložení pozic všech objektů na displeji
    static int newX = 0; // Nová X pozice kurzoru
    static int newY = 0; // Nová Y pozice kurzoru

    // Kontrola pohybu kurzoru - kurz se pohne pouze pokud je vstup a uplynul čas
    if(movX != 0 || movY != 0) { // Pokud je nějaký vstup z joysticku
        if (moveTimer.hasTimePassed(100)) // Pokud uplynulo 100ms od poslední změny pozice
        {
            newX = cursor->getXPos() + movX; // Vypočítej novou X pozici
            newY = cursor->getYPos() + movY; // Vypočítej novou Y pozici
        }
    }

    // Omezení pohybu kurzoru na herní pole (1-6 na obou osách)
    if(newX < 1) { // Pokud by kurzor šel vlevo mimo pole
        newX = 1; // Postav ho na levý okraj
    }else if(newX > 6) { // Pokud by kurzor šel vpravo mimo pole
        newX = 6; // Postav ho na pravý okraj
    }
    if(newY < 1) { // Pokud by kurzor šel nahoru mimo pole
        newY = 1; // Postav ho na horní okraj
    }else if(newY > 6) { // Pokud by kurzor šel dolů mimo pole
        newY = 6; // Postav ho na dolní okraj
    }
    
    // Pohyb projektilu - rychlost se zvyšuje s obtížností
    if(timer->hasTimePassed(500/level)) { // Pokud je čas na pohyb projektilu (při vyšší obtížnosti je interval kratší)
        projectile->move(); // Posuň projektil
    }

    // Umístění projektilu na mapu
    positions[projectile->getYPos()][projectile->getXPos()] = 1; // Označ pozici projektilu

    // Aktualizace pozice kurzoru
    cursor->setXPos(newX); // Nastav novou X pozici
    cursor->setYPos(newY); // Nastav novou Y pozici
    display->clear(); // Vymaž displej
    
    // Vykreslení kurzoru jako kříže (4 ledky kolem středu)
    positions[cursor->getYPos()+1][cursor->getXPos()] = !positions[cursor->getYPos()+1][cursor->getXPos()]; // Dolní část kříže
    positions[cursor->getYPos()-1][cursor->getXPos()] = !positions[cursor->getYPos()-1][cursor->getXPos()]; // Horní část kříže
    positions[cursor->getYPos()][cursor->getXPos()-1] = !positions[cursor->getYPos()][cursor->getXPos()-1]; // Levá část kříže
    positions[cursor->getYPos()][cursor->getXPos()+1] = !positions[cursor->getYPos()][cursor->getXPos()+1]; // Pravá část kříže
    

    // Vykreslení všech řádků na displej
    for(int i = 0; i < 8; i++) { // Procházej všechny řádky matice
        int rowValue = 0; // Hodnota pro daný řádek (8 bitů = 8 LEDek)
        for(int j = 0; j < 8; j++) { // Procházej všechny sloupce
            rowValue |= (positions[i][j] << (7 - j)); // Vytvoř bitovou reprezentaci řádku
        }
        display->setRow(i + 1, rowValue, 1); // Vypiš řádek na displej
    }

    // Kontrola stisku tlačítka a detekce kolize
    if(switchState == 1) { // Pokud je tlačítko joysticku stisknuto
       if(cursor->getXPos() == projectile->getXPos() && cursor->getYPos() == projectile->getYPos()) { // Pokud se kurzor shoduje s pozicí projektilu (zásah)
            //Serial.println("\033[2J"); // Vymaž konzolu
            //Serial.println("Hit!"); // Zpráva o zásahu
            (*pointPTR)++; // Přidej bod
            //Serial.printf("Current points: %lu\n", *pointPTR); // Vytiskni body
            projectile->RandStart(); // Nastav projektil na novou náhodnou pozici
        }
    }
}

/**
 * Konstruktor třídy MyTimer - inicializuje timer
 */
MyTimer::MyTimer() {
    timeOfChange = 0; // Na začátku je čas resetnut
}

/**
 * Kontroluje, zda uplynul zadaný čas od poslední kontroly
 * @param time - čas v milisekundách
 * @return true pokud uplynul zadaný čas, false jinak
 */
bool MyTimer::hasTimePassed(unsigned long time) {
    if(this->timeOfChange == 0) this->timeOfChange = millis() + time; // Pokud je timer prázdný, nastav startovní čas
    if(millis() < this->timeOfChange) { // Pokud aktuální čas je menší než cílový čas
        return false; // Čas ještě neuplynul
    }else {
        this->timeOfChange = 0; // Reset timeru
        return true; // Čas uplynul
    }
}

/**
 * Konstruktor třídy Projectile - inicializuje projektil
 * @param x - počáteční X pozice
 * @param y - počáteční Y pozice
 */
Projectile::Projectile(int x, int y) {
    xPos = x; // Ulož X pozici
    yPos = y; // Ulož Y pozici
}

/**
 * Vrátí aktuální X pozici projektilu
 * @return X pozice
 */
int Projectile::getXPos() {
    return xPos; // Vrátí X pozici
}

/**
 * Vrátí aktuální Y pozici projektilu
 * @return Y pozice
 */
int Projectile::getYPos() {
    return yPos; // Vrátí Y pozici
}

/**
 * Pohyb projektilu - aktualizuje pozici a zpracovává odrazy od stěn
 */
void Projectile::move() {
    xPos += xDir; // Posuň projektil na ose X
    yPos += yDir; // Posuň projektil na ose Y

    // Kontrola kolize se stěnami a odraz projektilu
    if(DirState::UP == dir) { // Pokud se projektil pohybuje nahoru
        if(yPos < 0) { // Pokud by přešel horní okraj
            yPos = 0; // Postav ho na okraj
            dir = DirState::DOWN; // Změň směr na dolů
            yDir = -yDir; // Invertuj směr na ose Y
        }
    } else if(DirState::DOWN == dir) { // Pokud se projektil pohybuje dolů
        if(yPos > 7) { // Pokud by přešel dolní okraj
            yPos = 7; // Postav ho na okraj
            dir = DirState::UP; // Změň směr na nahoru
            yDir = -yDir; // Invertuj směr na ose Y
        }
    } else if(DirState::LEFT == dir) { // Pokud se projektil pohybuje vlevo
        if(xPos < 0) { // Pokud by přešel levý okraj
            xPos = 0; // Postav ho na okraj
            dir = DirState::RIGHT; // Změň směr vpravo
            xDir = -xDir; // Invertuj směr na ose X
        }
    } else if(DirState::RIGHT == dir) { // Pokud se projektil pohybuje vpravo
        if(xPos > 7) { // Pokud by přešel pravý okraj
            xPos = 7; // Postav ho na okraj
            dir = DirState::LEFT; // Změň směr vlevo
            xDir = -xDir; // Invertuj směr na ose X
        }
    }
}

/**
 * Nastavení X pozice projektilu
 * @param xDir - nová X pozice
 */
void Projectile::setXPos(int xDir) {
    this->xPos = xDir; // Ulož novou X pozici
}

/**
 * Nastavení Y pozice projektilu
 * @param yDir - nová Y pozice
 */
void Projectile::setYPos(int yDir) {
    this->yPos = yDir; // Ulož novou Y pozici
}

/**
 * Nastav projektil na náhodnou pozici a směr
 * - projektil se vždy spustí od jedné ze stěn směrem do hracího pole
 */
void Projectile::RandStart() {
    this->dir = intToDirState(random(0, 4)); // Vyber náhodný směr (0-3)
    switch(this->dir) { // Podle zvoleného směru nastav počáteční pozici a věktor pohybu
        case UP: // Projektil má jít nahoru
            xPos = random(1, 6); // Náhodná X pozice
            yPos = 0; // Počáteční pozice na dolním okraji
            xDir = 0; // Bez pohybu na ose X
            yDir = -1; // Pohyb nahoru
            break;
        case DOWN: // Projektil má jít dolů
            xPos = random(1, 6); // Náhodná X pozice
            yPos = 7; // Počáteční pozice na horním okraji
            xDir = 0; // Bez pohybu na ose X
            yDir = 1; // Pohyb dolů
            break;
        case LEFT: // Projektil má jít vlevo
            xPos = 7; // Počáteční pozice na pravém okraji
            yPos = random(1, 6); // Náhodná Y pozice
            xDir = -1; // Pohyb vlevo
            yDir = 0; // Bez pohybu na ose Y
            break;
        case RIGHT: // Projektil má jít vpravo
            xPos = 0; // Počáteční pozice na levém okraji
            yPos = random(1, 6); // Náhodná Y pozice
            xDir = 1; // Pohyb vpravo
            yDir = 0; // Bez pohybu na ose Y
            break;
        default: // Fallback (nemělo by se stát)
            xPos = random(1, 6); // Náhodná X pozice
            yPos = random(1, 6); // Náhodná Y pozice
            xDir = 0; // Bez pohybu
            yDir = 0; // Bez pohybu
    }
}

/**
 * Převede celé číslo na enum DirState (směr)
 * @param number - číslo 0-3 pro směr (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
 * @return odpovídající DirState
 */
DirState intToDirState(int number){
    switch(number){ // Převeď číslo na odpovídající směr
        case 0: return UP; // 0 = nahoru
        case 1: return DOWN; // 1 = dolů
        case 2: return LEFT; // 2 = vlevo
        case 3: return RIGHT; // 3 = vpravo
        default: return NONE; // Neurčitý směr
    }
}

/**
 * Krásné vypnutí displeje - blikací animace
 * @param display - ukazatel na LED matici
 */
void resetDisplay(MATRIX7219* display){ // Vypnutí s animací
    // Nejdřív zhasni všechny LEDky
    for(int i = 1; i <= 8; i++){ // Projdi všechny řádky
        display->setRow(i,0,1); // Zhasni všechny LEDky v řádku
    }

    // Alternující blikání pro efekt
    for(int i = 1; i <= 8; i++){ // Projdi všechny řádky
        display->setRow(i,255,1); // Rozsvít všechny LEDky v řádku
        delay(100); // Čekej 100ms
    }

    // Finální vypnutí
    for(int i = 1; i <= 8; i++){ // Projdi všechny řádky
        display->setRow(i,0,1); // Zhasni všechny LEDky v řádku
        delay(100); // Čekej 100ms
    }
}

/**
 * Test bzučáku - krátkým tónem
 */
void testBuzzer(){ // Test zvukového signalizace
    digitalWrite(BUZZ_PIN, HIGH); // Zapni bzučák
    delay(200); // Pusť zvuk na 200ms
    digitalWrite(BUZZ_PIN, LOW); // Vypni bzučák
}