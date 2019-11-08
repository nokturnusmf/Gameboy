#include "display.h"

#include "memmap.h"
#include "interrupts.h"

static const int ModeCycles[] = { 80, 172, 204 };
static const int SCANLINE = ModeCycles[0] + ModeCycles[1] + ModeCycles[2];

static const byte STAT_MASK = 0xFC;
static const byte LYC_IE = 0x40;
static const byte LYC_COINC = 0x04;

static const byte LCD_ENABLE = 0x80;
static const byte WINDOW_TILE_MAP = 0x40;
static const byte WINDOW_ENABLE = 0x20;
static const byte TILE_DATA_SELECT = 0x10;
static const byte BG_TILE_MAP = 0x08;
static const byte SPRITE_SIZE = 0x04;
static const byte SPRITE_ENABLE = 0x02;
static const byte BG_ENABLE = 0x01;

void Display::advance(long cycles) {
    cycle_count += cycles;
    switch (mode) {
    case VideoMode::Oam:
        if (cycle_count >= ModeCycles[mode]) {
            cycle_count %= ModeCycles[mode];
            set_mode(VideoMode::Transfer);
        }
        break;

    case VideoMode::Transfer:
        if (cycle_count >= ModeCycles[mode]) {
            cycle_count %= ModeCycles[mode];
            set_mode(VideoMode::HBlank);

            if (regs.stat & 0x08) {
                interrupts.flags |= Interrupts::LCDStat;
            }

            if (regs.stat & LYC_IE && regs.ly == regs.lyc) {
                interrupts.flags |= Interrupts::LCDStat;
                regs.stat |= LYC_COINC;
            } else {
                regs.stat &= ~LYC_COINC;
            }
        }
        break;

    case VideoMode::HBlank:
        if (cycle_count >= ModeCycles[mode]) {
            cycle_count %= ModeCycles[mode];

            draw_line(regs.ly);

            if (++regs.ly < 144) {
                set_mode(VideoMode::Oam);
            } else {
                set_mode(VideoMode::VBlank);
                interrupts.flags |= Interrupts::VBlank;
            }
        }
        break;

    case VideoMode::VBlank:
        if (cycle_count >= SCANLINE) {
            cycle_count %= SCANLINE;

            if (++regs.ly == 154) {
                if (display_enabled()) {
                    draw_sprites();
                    write_frame();
                }

                set_mode(VideoMode::Oam);
                regs.ly = 0;
            }
        }
        break;
    }
}

inline bool Display::display_enabled() const {
    return regs.lcdc & LCD_ENABLE;
}

inline bool Display::bg_enabled() const {
    return regs.lcdc & BG_ENABLE;
}

inline bool Display::window_enabled() const {
    return regs.lcdc & WINDOW_ENABLE;
}

inline bool Display::sprites_enabled() const {
    return regs.lcdc & SPRITE_ENABLE;
}

void Display::set_mode(VideoMode new_mode) {
    byte b;
    switch (new_mode) {
    case VideoMode::Oam:
        b = 2;
        break;
    case VideoMode::Transfer:
        b = 3;
        break;
    case VideoMode::HBlank:
        b = 0;
        break;
    case VideoMode::VBlank:
        b = 1;
        break;
    }
    regs.stat = (regs.stat & STAT_MASK) | b;
    mode = new_mode;
}

void Display::draw_line(int line) {
    if (bg_enabled()) {
        draw_bg_line(line);
    }

    if (window_enabled()) {
        draw_window_line(line);
    }
}

void Display::draw_bg_line(int line) {
    word tile_map = regs.lcdc & BG_TILE_MAP ? 0x9C00 : 0x9800;
    word tile_data = regs.lcdc & TILE_DATA_SELECT ? 0x8800 : 0x8000;

    byte row = line + regs.scy;

    for (int i = 0; i < 160 + 8; i += 8) {
        byte column = i + regs.scx;

        byte tile_index = memmap.read(tile_map + row * 4 + column / 8);
        word tile = memmap.read_word(tile_index);

        word data = memmap.read_word(tile_data + tile + row % 8);

        draw_pixel_line(data, i - regs.scx, line, false);
    }
}

void Display::draw_window_line(int line) {
    word tile_map = regs.lcdc & WINDOW_TILE_MAP ? 0x9C00 : 0x9800;
    word tile_data = regs.lcdc & TILE_DATA_SELECT ? 0x8800 : 0x8000;

    byte row = line + regs.wy;

    for (int i = -7; i < 160; i += 8) {
        byte column = i + regs.wx;

        byte tile_index = memmap.read(tile_map + row * 4 + column / 8);
        word tile = memmap.read_word(tile_data + tile_index + row % 8);

        draw_pixel_line(tile, i - regs.wx, line, false);
    }
}

void Display::draw_sprites() {
    if (sprites_enabled()) {
        for (int i = 0; i < 40; ++i) {
            draw_sprite(i);
        }
    }
}

void Display::draw_sprite(int n) {
    byte y = memmap.read(0xFE00 + n * 4);
    byte x = memmap.read(0xFE01 + n * 4);
    byte tile_index = memmap.read(0xFE02 + n * 4);
    // byte attr = memmap.read(0xFE03 + n * 4);

    if (y == 0 || y >= 160 || x == 0 || x > 168) return;

    for (int i = 0; i < 8; ++i) {
        word data = memmap.read_word(0x8000 + 2 * tile_index);
        draw_pixel_line(data, x, y + i, true);
    }
}

void Display::draw_pixel_line(word pixel, int x, int y, bool is_sprite) {
    if (y < 0 || y >= 144) return;

    byte upper = pixel >> 8, lower = pixel;
    for (int i = 0; i < 8; ++i) {
        byte color = ((upper) & 0x80) >> 6 | (lower >> 7);
        upper <<= 1; lower <<= 1;

        if (x + i < 0) continue;
        if (x + i >= 160) break;

        if (is_sprite && !color) continue;

        Pixel p = get_pixel(color);
        pixels[y * 160 + x + i] = p;
    }
}

Pixel Display::get_pixel(byte index) {
    switch (index) {
    case 0:
        return { 0, 0, 0, 0 };
    case 1:
        return { 64, 64, 64, 0 };
    case 2:
        return { 144, 144, 144, 0 };
    case 3:
        return { 255, 255, 255, 0 };
    }
    throw index;
}

void Display::write_frame()  {
    for (int i = 0; i < 144; ++i) {
        for (int j = 0; j < 160; ++j) {
            // draw_pixel_line((i + j) * i * j, j, i, false);
        }
    }

    byte frame[160 * 144 * 3];
    for (int i = 0; i < 160 * 144; ++i) {
        Pixel p = pixels[i];
        frame[i * 3 + 0] = p.r;
        frame[i * 3 + 1] = p.g;
        frame[i * 3 + 2] = p.b;
    }
    display_callback(frame);
}

#include <iostream>
#include <iomanip>

byte Display::read_io(word address) {
    if (address == 0xFF44) return regs.ly;
    std::cout << "lcd read " << std::hex << address << '\n';
    switch (address & 0xFF) {
    case 0x40:
        return regs.lcdc;
    case 0x41:
        return regs.stat;
    case 0x42:
        return regs.scy;
    case 0x43:
        return regs.scx;
    case 0x44:
        return regs.ly;
    case 0x45:
        return regs.lyc;
    case 0x4A:
        return regs.wy;
    case 0x4B:
        return regs.wx;
    case 0x68:
        return regs.bgpi;
    case 0x69:
        return regs.bgpd;
    case 0x6A:
        return regs.obpi;
    case 0x6B:
        return regs.obpd;
    default:
        std::cerr << "lcd invalid read " << address << '\n';
        return 0;
    }
}

void Display::write_io(word address, byte value) {
    std::cout << "lcd write " << std::hex << address << ' ' << (int)value << '\n';
    switch (address & 0xFF) {
    case 0x40:
        regs.lcdc = value;
        break;
    case 0x41:
        regs.stat = value;
        break;
    case 0x42:
        regs.scy = value;
        break;
    case 0x43:
        regs.scx = value;
        break;
    case 0x44:
        regs.ly = value;
        break;
    case 0x45:
        regs.lyc = value;
        break;
    case 0x4A:
        regs.wy = value;
        break;
    case 0x4B:
        regs.wx = value;
        break;
    case 0x68:
        regs.bgpi = value;
        break;
    case 0x69:
        regs.bgpd = value;
        break;
    case 0x6A:
        regs.obpi = value;
        break;
    case 0x6B:
        regs.obpd = value;
        break;
    default:
        std::cerr << "lcd invalid write " << address << ' ' << (int)value << '\n';
    }
}
