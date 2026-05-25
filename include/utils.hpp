#ifndef UTILS_HPP
#define UTILS_HPP

//encoder a is fucking clock
//encoder b is fucking data
//encoder sw is fucking button
/**
 * defines
 */
#define DATA_PIN 14
#define CLOCK_PIN 27
#define SELECT_PIN 26
#define NUM_DEVICES 1
#define BUZZ_PIN 22
#define ENCODE_CLOCK_PIN 5
#define ENCODE_DATA_PIN 18
#define ENCODE_SW_PIN 19
#define JOYSTICK_SW 32
#define JOYSTICK_X 34
#define JOYSTICK_Y 35
/**
 * An enum representing the possible directions of movement for the projectile. It is used to determine how the projectile moves when the move function is called.
 */
enum DirState{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};
/**
 * A class representing the cursor in the game. It has a position.
 */
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
/**
 * A class representing the projectile in the game. It has a position and a direction. The move function updates the position based on the direction. The RandStart function sets a random starting position and direction for the projectile.
 */
class Projectile{
    private:
        int xPos;
        int yPos;
        int xDir = 0;
        int yDir = 0;
        DirState dir;
    public:
        Projectile(int x = 0, int y = 0);
        int getXPos();
        int getYPos();
        void setXPos(int xDir);
        void setYPos(int yDir);
        void move();
        void RandStart();
};
/**
 * A simple timer class that can be used to check if a certain amount of time has passed since the last change. It is used to control the speed of the projectile and the movement of the cursor.
 */
class MyTimer{
    private:
        unsigned long timeOfChange = 0;
    public:
        MyTimer();
        bool hasTimePassed(unsigned long time);
};
/**
 * @param display - pointer to the display object
 */
void testDisplay(MATRIX7219* display);
void testBuzzer();
/**
 * @param display - pointer to the display object
 * @param cursor - pointer to the cursor object
 * @param movX -1, 0 or 1 depending on the direction of movement
 * @param movY -1, 0 or 1 depending on the direction of movement
 * @param level 1-10 depending on the difficulty level, determines the speed of the projectile
 * @param switchState - state of the switch
 * @param pointPTR - pointer to the points variable
 */
void displayGame(MATRIX7219* display, Cursor* cursor, Projectile* projectile, MyTimer *timer, int movX, int movY, unsigned long *pointPTR, int level = 1, int switchState = 0);
void resetDisplay(MATRIX7219* display);
DirState intToDirState(int number);
#endif // UTILS_HPP