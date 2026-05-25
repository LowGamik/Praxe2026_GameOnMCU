#ifndef UTILS_HPP
#define UTILS_HPP

//encoder a is fucking clock
//encoder b is fucking data
//encoder sw is fucking button
/**
 * Definice pinů - mapování hardwarových pinů ESP32
 */
#define DATA_PIN 14 // Pin pro datový signál LED matice (DIN)
#define CLOCK_PIN 27 // Pin pro hodinový signál LED matice (CLK)
#define SELECT_PIN 26 // Pin pro výběr zařízení LED matice (CS)
#define NUM_DEVICES 1 // Počet řetězených LED matrixů
#define BUZZ_PIN 22 // Pin pro bzučák (audio signalizace)
#define ENCODE_CLOCK_PIN 5 // Pin pro hodinový signál enkodéru (CLK)
#define ENCODE_DATA_PIN 18 // Pin pro datový signál enkodéru (DT)
#define ENCODE_SW_PIN 19 // Pin pro tlačítko enkodéru
#define JOYSTICK_SW 32 // Pin pro tlačítko joysticku
#define JOYSTICK_X 34 // Pin pro osu X joysticku (analogový)
#define JOYSTICK_Y 35 // Pin pro osu Y joysticku (analogový)
/**
 * Výčtový typ pro směry pohybu projektilu
 * - UP: projekil se pohybuje nahoru
 * - DOWN: projektil se pohybuje dolů
 * - LEFT: projektil se pohybuje vlevo
 * - RIGHT: projektil se pohybuje vpravo
 * - NONE: neurčitý nebo bez pohybu
 */
enum DirState{
    UP, // Nahoru
    DOWN, // Dolů
    LEFT, // Vlevo
    RIGHT, // Vpravo
    NONE // Bez směru
};
/**
 * Třída reprezentující kurzor (hráče) v herním poli
 * - Má pozici X a Y na matici 8x8
 * - Vrátí se jako kříž na displeji
 */
class Cursor{
    private:
        int xPos; // X pozice kurzoru
        int yPos; // Y pozice kurzoru
    public:
        Cursor(int x = 4, int y = 4); // Konstruktor s výchozí pozicí uprostřed
        void setXPos(int x); // Nastav novou X pozici
        void setYPos(int y); // Nastav novou Y pozici
        int getXPos(); // Vrátí aktuální X pozici
        int getYPos(); // Vrátí aktuální Y pozici
};
/**
 * Třída reprezentující projektil (cíl) v herním poli
 * - Má pozici X a Y na matici 8x8
 * - Pohybuje se určitým směrem (UP, DOWN, LEFT, RIGHT)
 * - Odráží se od stěn herního pole
 */
class Projectile{
    private:
        int xPos; // X pozice projektilu
        int yPos; // Y pozice projektilu
        int xDir = 0; // Směr pohybu na ose X (-1, 0, 1)
        int yDir = 0; // Směr pohybu na ose Y (-1, 0, 1)
        DirState dir; // Aktuální směr pohybu
    public:
        Projectile(int x = 0, int y = 0); // Konstruktor
        int getXPos(); // Vrátí aktuální X pozici
        int getYPos(); // Vrátí aktuální Y pozici
        void setXPos(int xDir); // Nastav novou X pozici
        void setYPos(int yDir); // Nastav novou Y pozici
        void move(); // Posuň projektil a zpracuj odrazy
        void RandStart(); // Nastav na náhodnou pozici a směr
};
/**
 * Jednoduchá třída pro měření času
 * - Sleduje, zda uplynul zadaný počet milisekund
 * - Používá se pro řízení rychlosti pohybu kurzoru a projektilu
 */
class MyTimer{
    private:
        unsigned long timeOfChange = 0; // Čas, kdy by mělo skončit čekání
    public:
        MyTimer(); // Konstruktor
        bool hasTimePassed(unsigned long time); // Vrátí true pokud uplynul zadaný čas
};
/**
 * Testuje funkcionalitu LED matice - postupně rozsvěcuje řádky
 * @param display - ukazatel na objekt LED matice
 */
void testDisplay(MATRIX7219* display);

/**
 * Testuje buzzer - krátký zvukový signál
 */
void testBuzzer();

/**
 * Hlavní herní funkce - aktualizuje a vykresluje herní scénu
 * @param display - ukazatel na LED matici pro vykreslení
 * @param cursor - ukazatel na kurzor (hráč)
 * @param projectile - ukazatel na projektil (cíl)
 * @param timer - ukazatel na timer pro pohyb projektilu
 * @param movX - směr pohybu na ose X (-1 vlevo, 0 bez pohybu, 1 vpravo)
 * @param movY - směr pohybu na ose Y (-1 nahoru, 0 bez pohybu, 1 dolů)
 * @param pointPTR - ukazatel na proměnnou s počtem bodů
 * @param level - obtížnost hry 1-10 (určuje rychlost projektilu)
 * @param switchState - stav tlačítka joysticku (pro detekci zásahu)
 */
void displayGame(MATRIX7219* display, Cursor* cursor, Projectile* projectile, MyTimer *timer, int movX, int movY, unsigned long *pointPTR, int level = 1, int switchState = 0);

/**
 * Vypnutí displeje s krásnou animací
 * @param display - ukazatel na LED matici
 */
void resetDisplay(MATRIX7219* display);

/**
 * Pomocná funkce pro převod celého čísla na směr (DirState)
 * @param number - číslo 0-3 (0=UP, 1=DOWN, 2=LEFT, 3=RIGHT)
 * @return odpovídající DirState
 */
DirState intToDirState(int number);
#endif // UTILS_HPP