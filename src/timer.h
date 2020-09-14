#pragma once

#include "types.h"
#include "interrupts.h"

class Timer {
public:
    Timer(Interrupts* interrupts) : interrupts(interrupts) {}

    void update(int cycles);

    byte div = 0;

    byte tima = 0;
    byte tma = 0;

    byte tac = 0;

private:
    Interrupts* interrupts;

    int div_counter = 0;
    int tim_counter = 0;
};
