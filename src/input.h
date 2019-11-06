#pragma once

#include "gb.h"

class Input {
public:
    enum Button {
        Down,
        Up,
        Left,
        Right,
        Start,
        Select,
        B,
        A
    };

    void press(Button button);
    void release(Button button);

    void write(byte b);
    byte read() const;

private:
    bool states[8];
    bool toggle;
};
