#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H

#include "mbed.h"

class SevenSegmentDisplay {
public:
    // Constructor: accepts GPIO pins for segments a–g (in order!)
    SevenSegmentDisplay(PinName a, PinName b, PinName c, PinName d,
                        PinName e, PinName f, PinName g);

    // Display a digit from 0–9 on the 7-segment display
    void display_digit(int digit);

private:
    DigitalOut _a, _b, _c, _d, _e, _f, _g;

    // Segment bitmaps for digits 0–9, order is a–g
    const int digit_map[10][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };
};

#endif
