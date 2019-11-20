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

Display::Display(MemoryMap& memmap, Interrupts& interrupts, void(*display_callback)(byte*))
    : memmap(memmap), interrupts(interrupts), display_callback(display_callback), regs{} {
}

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
    word tile_data = regs.lcdc & TILE_DATA_SELECT ? 0x8000 : 0x9000;

    byte row = line + regs.scy;

    for (int i = 0; i < 160 + 8; i += 8) {
        byte column = i + regs.scx;

        int tile_index = memmap.read(tile_map + (row & ~7u) * 4 + column / 8);
        if (tile_data == 0x9000 && tile_index >= 128) {
            tile_index -= 256;
        }

        word data = memmap.read_word(tile_data + tile_index * 16 + (row % 8) * 2);

        draw_pixel_line(data, i, line, false, false, false);
    }
}

void Display::draw_window_line(int line) {
    word tile_map = regs.lcdc & WINDOW_TILE_MAP ? 0x9C00 : 0x9800;
    word tile_data = regs.lcdc & TILE_DATA_SELECT ? 0x8000 : 0x9000;

    byte row = line + regs.wy;

    for (int i = 0; i < 160 + 8; i += 8) {
        byte column = i + regs.wx - 7;

        int tile_index = memmap.read(tile_map + (row & ~7u) * 4 + column / 8);
        if (tile_data == 0x9000 && tile_index >= 128) {
            tile_index -= 256;
        }

        word data = memmap.read_word(tile_data + tile_index * 16 + (row % 8) * 2);

        draw_pixel_line(data, i, line, false, false, false);
    }
}

void Display::draw_sprites() {
    if (sprites_enabled()) {
        for (int i = 0; i < 40; ++i) {
            draw_sprite(i);
        }
    }
}

inline word flip_pixel_line(word w) {
    w = (w & 0xF0F0) >> 4 | (w & 0x0F0F) << 4;
    w = (w & 0xCCCC) >> 2 | (w & 0x3333) << 2;
    w = (w & 0xAAAA) >> 1 | (w & 0x5555) << 1;
    return w;
}

void Display::draw_sprite(int n) {
    byte y = memmap.read(0xFE00 + n * 4);
    byte x = memmap.read(0xFE01 + n * 4);
    byte tile_index = memmap.read(0xFE02 + n * 4);
    byte attr = memmap.read(0xFE03 + n * 4);

    if (y == 0 || y >= 144 || x == 0 || x > 168) return;

    for (int i = 0; i < 8; ++i) {
        word data = memmap.read_word(0x8000 + tile_index * 16 + (attr & 0x40 ? 7 - i : i) * 2);
        if (attr & 0x20) {
            data = flip_pixel_line(data);
        }
        draw_pixel_line(data, x - 8, y + i - 16, true, attr & 0x10, ~attr & 0x80);
    }
}

void Display::draw_pixel_line(word pixel, int x, int y, bool is_sprite, bool sprite_palette, bool sprite_priority) {
    if (y < 0 || y >= 144) return;

    byte upper = pixel >> 8, lower = pixel;
    for (int i = 0; i < 8; ++i) {
        byte color = ((upper) & 0x80) >> 6 | (lower >> 7);
        upper <<= 1; lower <<= 1;

        if (x + i < 0) continue;
        if (x + i >= 160) break;

        if (is_sprite && !color) continue;

        if (is_sprite && !sprite_priority && depth[y * 160 + x + i]) continue;

        frame[y * 160 + x + i] = get_pixel(color, is_sprite, sprite_palette);
        depth[y * 160 + x + i] = is_sprite | color;
    }
}

inline Pixel Display::get_pixel(byte index, bool is_sprite, bool sprite_palette) {
    byte palette = is_sprite ? sprite_palette ? regs.obp1 : regs.obp0 : regs.bgp;
    return map_pixel((palette & (0b11 << index * 2)) >> index * 2);
}

Pixel Display::map_pixel(byte index) {
    switch (index) {
    case 0:
        return { 255, 255, 255 };
    case 1:
        return { 144, 144, 144 };
    case 2:
        return { 64, 64, 64 };
    case 3:
        return { 0, 0, 0 };
    }
    throw index;
}

void Display::write_frame()  {
    display_callback(reinterpret_cast<byte*>(frame));
    for (auto& b : depth) b = 0;
}

byte Display::read_io(word address) {
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
    case 0x47:
        return regs.bgp;
    case 0x48:
        return regs.obp0;
    case 0x49:
        return regs.obp1;
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
        // std::cerr << "lcd invalid read " << address << '\n';
        return 0;
    }
}

void Display::write_io(word address, byte value) {
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
    case 0x47:
        regs.bgp = value;
        break;
    case 0x48:
        regs.obp0 = value;
        break;
    case 0x49:
        regs.obp1 = value;
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
        // std::cerr << "lcd invalid write " << address << ' ' << (int)value << '\n';
        ;
    }
}
