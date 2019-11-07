#pragma once

#include "gb.h"

class Interrupts;

struct Pixel {
    byte r;
    byte g;
    byte b;
};

enum VideoMode {
    Oam,
    Transfer,
    HBlank,
    VBlank
};

struct LCDRegisters {
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

class Display {
public:
    Display(void(*display_callback)(byte*), Interrupts& interrupts)
        : display_callback(display_callback), interrupts(interrupts) {}

    void advance(long cycles);

    byte read_io(word address);
    void write_io(word address, byte value);

private:
    void(*display_callback)(byte*);

    Interrupts& interrupts;

    VideoMode mode = VideoMode::Oam;
    LCDRegisters regs;

    Pixel framebuffer[160 * 144];

    long cycle_count = 0;
};
