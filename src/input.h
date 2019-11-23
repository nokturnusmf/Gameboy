#pragma once

#include "types.h"

class Input {
public:
    Input() : toggle(false) {}

    enum Button {
        Down,
        Up,
        Left,
        Right,
        Start,
        Select,
        B,
        A,
        None
    };

    void press(Button button);
    void release(Button button);

    void write(byte b);
    byte read() const;

private:
    bool states[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
    bool toggle;
};
