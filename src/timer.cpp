#include "timer.h"

static const int CYCLES_PER_DIV = 256;

static const int TIMER_ENABLE = 0x04;
static const int CYCLES_PER_CLOCK[] = { 1024, 16, 64, 256 };

void Timer::update(long cycles) {
    if (div_counter += cycles > CYCLES_PER_DIV) {
        div_counter -= CYCLES_PER_DIV;
        ++div;
    }

    if (tac & TIMER_ENABLE) {
        int ccc = CYCLES_PER_CLOCK[tac & 0b11];
        if (tim_counter += cycles >= ccc) {
            tim_counter -= ccc;
            interrupts.flags |= Interrupts::Timer;
        }
    }
}
