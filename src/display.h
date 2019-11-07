#pragma once

#include "gb.h"

class Display {
public:
    Display(void(*display_callback)(unsigned char*))
        : display_callback(display_callback) {}

    void advance(long cycles);

    byte read_io(word address);
    void write_io(word address, byte value);

private:
    void(*display_callback)(unsigned char*);

    unsigned char framebuffer[160 * 144 * 3];

    long cycle_count = 0;

    byte lcdc;
    byte stat;
    byte scy;
    byte scx;
    byte ly;
    byte lyc;
    byte wy;
    byte wx;
    byte bgpi;
    byte bgpd;
    byte obpi;
    byte obpd;
};
