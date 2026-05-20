#ifndef UTILS_HPP
#define UTILS_HPP

//encoder a is fucking clock
//encoder b is fucking data
//encoder sw is fucking button

#define DATA_PIN 14
#define CLOCK_PIN 27
#define SELECT_PIN 26
#define NUM_DEVICES 1
#define BUTTON_PIN 32
#define BUZZ_PIN 22
#define ENCODE_CLOCK_PIN 5
#define ENCODE_DATA_PIN 18
#define JOYSTICK_SW 32
#define JOYSTICK_X 34
#define JOYSTICK_Y 35

class Cursor{
    private:
        int xPos;
        int yPos;
    public:
        Cursor(int x = 4, int y = 4);
        void setXPos(int x);
        void setYPos(int y);
        int getXPos();
        int getYPos();
};

class Projectile{
    private:
        int xPos;
        int yPos;
    public:
        Projectile(int x = 0, int y = 0);
};

class MyTimer{
    private:
        unsigned long timeOfChange;
    public:
        MyTimer();
        bool hasTimePassed(unsigned long time);
};

void testDisplay(MATRIX7219* display);
void displayGame(MATRIX7219* display, Cursor* cursor, int movX, int movY);

#endif // UTILS_HPP