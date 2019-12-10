#pragma once

#include "types.h"

struct Interrupts;
class MemoryMap;

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
    Display(MemoryMap& memmap, Interrupts& interrupts, bool(*display_callback)(byte*));

    void advance(int cycles);

    byte read_io(word address);
    void write_io(word address, byte value);

    bool close_requested() const { return !open; }

private:
    void set_mode(VideoMode mode);

    bool display_enabled() const;
    bool bg_enabled() const;
    bool window_enabled() const;
    bool sprites_enabled() const;

    void draw_scanline(int line);

    void draw_bg_line(int line);
    void draw_window_line(int line);
    void draw_line(int x, int y, byte row, byte column, word tile_map, word tile_data);

    void draw_sprites();
    void draw_sprite(int n);

    void draw_pixel_line(word pixel, int x, int y, bool is_sprite, bool sprite_palette, bool sprite_priority);

    Pixel get_pixel(byte index, bool is_sprite, bool sprite_palette);
    Pixel map_pixel(byte index);

    void write_frame();

    MemoryMap& memmap;
    Interrupts& interrupts;

    bool(*display_callback)(byte*);
    bool open = true;

    VideoMode mode;
    LCDRegisters regs;

    Pixel frame[160 * 144];
    bool depth[160 * 144];

    int cycle_count = 0;
};
