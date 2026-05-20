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


void testDisplay(MATRIX7219* display);

#endif // UTILS_HPP