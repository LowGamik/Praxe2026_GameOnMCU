#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

/**
 * Třída pro ovládání analogového joysticku s tlačítkem
 * - Měří polohu osy X a Y (0-4095)
 * - Detekuje stisnutí tlačítka
 */
class Joystick{
    private:
        int xValue; // Analogová hodnota osy X (0-4095)
        int yValue; // Analogová hodnota osy Y (0-4095)
        bool swValue; // Stav tlačítka (true=stisknuto, false=neuvolneno)
        int pinX; // Pin pro osu X
        int pinY; // Pin pro osu Y
        int pinSW; // Pin pro tlačítko
    public:
        Joystick(int pinX, int pinY, int pinSW); // Konstruktor - nastaví piny a režimy
        void update(); // Aktualizuj všechny hodnoty z joysticku
        int getX(); // Vrátí normalizovanou hodnotu osy X (-1, 0, 1)
        int getY(); // Vrátí normalizovanou hodnotu osy Y (-1, 0, 1)
        bool getSW(); // Vrátí stav tlačítka
};

#endif // JOYSTICK_HPP