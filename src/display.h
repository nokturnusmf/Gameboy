#pragma once

#include "gb.h"

class MemoryMap;
class Interrupts;

struct Pixel {
    byte r;
    byte g;
    byte b;
    byte depth;
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
    Display(MemoryMap& memmap, Interrupts& interrupts, void(*display_callback)(byte*))
        : memmap(memmap), interrupts(interrupts), display_callback(display_callback) {
        regs.ly = 145;
    }

    void advance(long cycles);

    byte read_io(word address);
    void write_io(word address, byte value);

private:
    void set_mode(VideoMode mode);

    bool display_enabled() const;
    bool bg_enabled() const;
    bool window_enabled() const;
    bool sprites_enabled() const;

    void draw_line(int line);

    void draw_bg_line(int line);
    void draw_window_line(int line);

    void draw_sprites();
    void draw_sprite(int n);

    void draw_pixel_line(word pixel, int x, int y, bool is_sprite);

    Pixel get_pixel(byte index);

    void write_frame() ;

    MemoryMap& memmap;
    Interrupts& interrupts;

    void(*display_callback)(byte*);

    VideoMode mode = VideoMode::VBlank;
    LCDRegisters regs;

    Pixel pixels[160 * 144];

    long cycle_count = 0;
};
