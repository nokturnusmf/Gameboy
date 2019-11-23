#pragma once

#include "types.h"
#include "interrupts.h"

class Timer {
public:
    Timer(Interrupts& interrupts) : interrupts(interrupts) {}

    void update(long cycles);

    byte div;

    byte tima = 0;
    byte tma = 0;

    byte tac = 0;

private:
    Interrupts& interrupts;

    long div_counter = 0;
    long tim_counter = 0;
};
