#include "SevenSegmentDisplay.h"

// Constructor
SevenSegmentDisplay::SevenSegmentDisplay(PinName a, PinName b, PinName c,
                                         PinName d, PinName e, PinName f, PinName g)
    : _a(a), _b(b), _c(c), _d(d), _e(e), _f(f), _g(g) {}

// Display one digit
void SevenSegmentDisplay::display_digit(int digit) {
    if (digit < 0 || digit > 9) return;

    _a = digit_map[digit][0];
    _b = digit_map[digit][1];
    _c = digit_map[digit][2];
    _d = digit_map[digit][3];
    _e = digit_map[digit][4];
    _f = digit_map[digit][5];
    _g = digit_map[digit][6];
}
