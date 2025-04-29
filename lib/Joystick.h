#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"

// Direction definitions
enum Direction {
    CENTRE, N, NE, E, SE, S, SW, W, NW
};

// Structure to represent joystick input
struct UserInput {
    Direction direction;
    float magnitude;
};

class Joystick {
public:
    Joystick(PinName vertPin, PinName horizPin);
    void init();                  // Calibrate center position
    Direction get_direction();    // Determine direction
    float get_mag();              // Get magnitude
    float get_angle();            // Get angle in degrees

private:
    AnalogIn *vert;
    AnalogIn *horiz;
    float _x0, _y0;               // Center calibration values
};

#endif
