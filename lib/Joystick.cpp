#include "Joystick.h"

Joystick::Joystick(PinName vertPin, PinName horizPin) {
    vert = new AnalogIn(vertPin);
    horiz = new AnalogIn(horizPin);
    _x0 = 0.5f;
    _y0 = 0.5f;
}

void Joystick::init() {
    // Dynamically calibrate center position
    _x0 = horiz->read();
    _y0 = vert->read();
}

Direction Joystick::get_direction() {
    float x = horiz->read() - _x0;
    float y = vert->read() - _y0;

    if (abs(x) < 0.1f && abs(y) < 0.1f) return CENTRE;
    if (x > 0.3f) return E;
    if (x < -0.3f) return W;
    if (y > 0.3f) return N;
    if (y < -0.3f) return S;

    return CENTRE;
}

float Joystick::get_mag() {
    float x = horiz->read() - _x0;
    float y = vert->read() - _y0;
    return sqrt(x * x + y * y);
}

float Joystick::get_angle() {
    float x = horiz->read() - _x0;
    float y = vert->read() - _y0;
    return atan2(y, x) * 180 / 3.14159;
}
