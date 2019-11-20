#include "input.h"

void Input::press(Button button) {
    states[button] = false;
}

void Input::release(Button button) {
    states[button] = true;
}

void Input::write(byte b) {
    toggle = b & 0x10;
}

byte Input::read() const {
    byte r = 0;
    for (int i = 0; i < 4; ++i) {
        r <<= 1;
        r |= states[i + 4 * toggle];
    }
    return r;
}
