#pragma once

#include "types.h"
#include "bank.h"

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
    byte wy;
    byte wx;
    byte bgpi;
    byte obpi;
    byte bgp[64];
    byte obp[64];
};

class Display {
public:
    Display(MemoryMap& memmap, Interrupts& interrupts, bool(*display_callback)(byte*));

    void advance(int cycles);

    byte read(word address) { return vram[address]; }
    void write(word address, byte value) { vram[address] = value; }

    byte read_io(byte reg);
    void write_io(byte reg, byte value);

    bool close_requested() const { return !open; }

private:
    void set_mode(VideoMode mode);
    word vram_word(word address);

    bool display_enabled() const;
    bool bg_enabled() const;
    bool window_enabled() const;
    bool sprites_enabled() const;

    void write_old_color(byte* palette, byte value);
    byte read_old_color(byte* palette);

    void draw_scanline(int line);

    void draw_bg_line(int line);
    void draw_window_line(int line);
    void draw_line(int x, int y, byte row, byte column, word tile_map, word tile_data);

    void draw_sprites();
    void draw_sprite(int n);

    void draw_pixel_line(int x, int y, word data, byte* palette, byte palette_index, bool is_sprite, bool sprite_priority);

    Pixel map_pixel(byte* palette, byte color);

    void write_frame();

    MemoryMap& memmap;
    Interrupts& interrupts;

    bool(*display_callback)(byte*);
    bool open = true;

    VideoMode mode;
    LCDRegisters regs;
    BankedMemory<0x8000, 0x2000, 0x0000> vram;

    Pixel frame[160 * 144];
    bool depth[160 * 144];

    int cycle_count = 0;
};
