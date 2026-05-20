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

enum DirState{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

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
        int xDir = 1;
        //int yDir;
        //DirState dir;
    public:
        Projectile(int x = 0, int y = 0/*DirState dir = NONE*/);
        int getXPos();
        int getYPos();
        void setXPos(int xDir);
        //void setYDir(int yDir);
        void move();
};

class MyTimer{
    private:
        unsigned long timeOfChange;
    public:
        MyTimer();
        bool hasTimePassed(unsigned long time);
};

void testDisplay(MATRIX7219* display);
/**¨
 * @param display - pointer to the display object
 * @param cursor - pointer to the cursor object
 * @param movX -1, 0 or 1 depending on the direction of movement
 * @param movY -1, 0 or 1 depending on the direction of movement
 * @param level 1-10 depending on the difficulty level, determines the speed of the projectile
 * 
 */
void displayGame(MATRIX7219* display, Cursor* cursor, Projectile* projectile, MyTimer *timer, int movX, int movY, int level = 1, int switchState = 0);

#endif // UTILS_HPP