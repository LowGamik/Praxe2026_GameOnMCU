#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

class Joystick{
    private:
        int xValue;
        int yValue;
        bool swValue;
        int pinX;
        int pinY;
        int pinSW;
    public:
        Joystick(int pinX, int pinY, int pinSW);
        void update();
        int getX();
        int getY();
        bool getSW();
};

#endif // JOYSTICK_HPP