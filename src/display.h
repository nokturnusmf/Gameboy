#pragma once

#include "gb.h"

class MemoryMap;
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
    byte bgp;
    byte obp0;
    byte obp1;
    byte wy;
    byte wx;
    byte bgpi;
    byte bgpd;
    byte obpi;
    byte obpd;
};

class Display {
public:
    Display(MemoryMap& memmap, Interrupts& interrupts, void(*display_callback)(byte*));

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

    void draw_pixel_line(word pixel, int x, int y, bool is_sprite, bool sprite_palette = false);

    Pixel get_pixel(byte index, bool is_sprite, bool sprite_palette = false);
    Pixel map_pixel(byte index);

    void write_frame() ;

    MemoryMap& memmap;
    Interrupts& interrupts;

    void(*display_callback)(byte*);

    VideoMode mode;
    LCDRegisters regs;

    Pixel frame[160 * 144];
    byte depth[160 * 144];

    long cycle_count = 0;
};
