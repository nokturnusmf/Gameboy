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
static const byte SPRITE_ENABLE = 0x02;
static const byte BG_ENABLE = 0x01;

Display::Display(MemoryMap& memmap, Interrupts& interrupts, bool(*display_callback)(byte*))
    : memmap(memmap), interrupts(interrupts), display_callback(display_callback), regs{}, vram(0x4000) {
}

void Display::advance(int cycles) {
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

            draw_scanline(regs.ly);

            if (++regs.ly < 144) {
                set_mode(VideoMode::Oam);
            } else {
                set_mode(VideoMode::VBlank);
                if (display_enabled()) {
                    interrupts.flags |= Interrupts::VBlank;
                }
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

inline word Display::vram_word(word address) {
    return (word)vram[address] | (word)vram[address + 1] << 8;
}

inline bool Display::display_enabled() const {
    return regs.lcdc & LCD_ENABLE;
}

inline bool Display::bg_enabled() const {
    return regs.lcdc & BG_ENABLE;
}

inline bool Display::window_enabled() const {
    return regs.lcdc & WINDOW_ENABLE && regs.wx < 167 && regs.wy < 144;
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

void Display::draw_scanline(int line) {
    int prev_bank = vram.get_bank();

    if (bg_enabled()) {
        draw_bg_line(line);
    }

    if (window_enabled()) {
        draw_window_line(line);
    }

    vram.set_bank(prev_bank);
}

void Display::draw_bg_line(int line) {
    word tile_map = regs.lcdc & BG_TILE_MAP ? 0x9C00 : 0x9800;
    word tile_data = regs.lcdc & TILE_DATA_SELECT ? 0x8000 : 0x9000;

    byte row = line + regs.scy;
    for (int i = 0; i < 168; i += 8) {
        draw_line(i - regs.scx % 8, line, row, i + regs.scx, tile_map, tile_data);
    }
}

void Display::draw_window_line(int line) {
    word tile_map = regs.lcdc & WINDOW_TILE_MAP ? 0x9C00 : 0x9800;
    word tile_data = regs.lcdc & TILE_DATA_SELECT ? 0x8000 : 0x9000;

    byte row = line + regs.wy;
    for (int i = 0; i < 168; i += 8) {
        draw_line(i - (regs.wx - 7) % 8, line, row, i + regs.wx - 7, tile_map, tile_data);
    }
}

inline word flip_pixel_line(word w) {
    w = (w & 0xF0F0) >> 4 | (w & 0x0F0F) << 4;
    w = (w & 0xCCCC) >> 2 | (w & 0x3333) << 2;
    w = (w & 0xAAAA) >> 1 | (w & 0x5555) << 1;
    return w;
}

void Display::draw_line(int x, int y, byte row, byte column, word tile_map, word tile_data) {
    vram.set_bank(0);
    int tile_index = vram[tile_map + (row & ~7u) * 4 + column / 8];
    if (tile_data == 0x9000 && tile_index >= 128) {
        tile_index -= 256;
    }

    vram.set_bank(1);
    byte attr = vram[tile_map + (row & ~7u) * 4 + column / 8];
    byte palette = attr & 0b111;
    bool bank = attr & 0x8;
    bool flip_y = attr & 0x20;

    vram.set_bank(bank);
    word data = vram_word(tile_data + tile_index * 16 + ((flip_y ? 7 - row % 8 : row % 8)) * 2);
    if (attr & 0x10) {
        data = flip_pixel_line(data);
    }

    draw_pixel_line(x, y, data, regs.bgp, palette, false, false);
}

void Display::draw_sprites() {
    if (sprites_enabled()) {
        int prev_bank = vram.get_bank();
        for (int i = 0; i < 40; ++i) {
            draw_sprite(i);
        }
        vram.set_bank(prev_bank);
    }
}

void Display::draw_sprite(int n) {
    byte y = memmap.read(0xFE00 + n * 4);
    byte x = memmap.read(0xFE01 + n * 4);
    byte tile_index = memmap.read(0xFE02 + n * 4);
    byte attr = memmap.read(0xFE03 + n * 4);

    if (y == 0 || y >= 160 || x == 0 || x >= 168) return;

    vram.set_bank((bool)(attr & 0x8));
    int sprite_h = regs.lcdc & 0b100 ? 16 : 8;
    for (int i = 0; i < sprite_h; ++i) {
        word data = vram_word(0x8000 + tile_index * 16 + (attr & 0x40 ? sprite_h - i - 1 : i) * 2);
        if (attr & 0x20) {
            data = flip_pixel_line(data);
        }
        draw_pixel_line(x - 8, y + i - 16, data, regs.obp, (bool)(attr & 0x10) | (attr & 0b111), true, ~attr & 0x80);
    }
}

void Display::draw_pixel_line(int x, int y, word data, byte* palette, byte palette_index, bool is_sprite, bool sprite_priority) {
    if (y < 0 || y >= 144) return;

    byte upper = data >> 8, lower = data;
    for (int i = 0; i < 8; ++i) {
        byte color = (upper & 0x80) >> 6 | (lower >> 7);
        upper <<= 1; lower <<= 1;

        if (x + i < 0) continue;
        if (x + i >= 160) break;

        if (is_sprite && (!color || (!sprite_priority && depth[y * 160 + x + i]))) continue;

        frame[y * 160 + x + i] = map_pixel(palette + 8 * palette_index, color);
        depth[y * 160 + x + i] = is_sprite | color;
    }
}

Pixel Display::map_pixel(byte* palette, byte color) {
    word w = palette[color * 2] | (palette[color * 2 + 1] << 8);
    return Pixel{ (byte)((w & 0x1F) << 3), (byte)((w & 0x3E0) >> 2), (byte)((w & 0x7C00) >> 7) };
}

void Display::write_frame()  {
    open = display_callback(reinterpret_cast<byte*>(frame));
    for (auto& b : depth) b = 0;
}

void Display::write_old_color(byte* palette, byte value) {
    static const word values[] = { 0x7FFF, 0x56B5, 0x2D6B, 0x0000 };
    word* p = reinterpret_cast<word*>(palette);
    for (int i = 0; i < 4; ++i) {
        p[i] = values[value & 0b11];
        value >>= 2;
    }
}

byte Display::read_io(byte reg) {
    switch (reg) {
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
        return regs.dmgp[0];
    case 0x48:
        return regs.dmgp[1];
    case 0x49:
        return regs.dmgp[2];
    case 0x4A:
        return regs.wy;
    case 0x4B:
        return regs.wx;
    case 0x4F:
        return vram.get_bank();
    case 0x68:
        return regs.bgpi;
    case 0x69:
        return regs.bgp[regs.bgpi & 0x3F];
    case 0x6A:
        return regs.obpi;
    case 0x6B:
        return regs.obp[regs.obpi & 0x3F];
    default:
        return 0xFF;
    }
}

void Display::write_io(byte reg, byte value) {
    switch (reg) {
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
        regs.dmgp[0] = value;
        write_old_color(regs.bgp, value);
        break;
    case 0x48:
        regs.dmgp[1] = value;
        write_old_color(regs.obp, value);
        for (int i = 16; i < 64; i += 8) {
            write_old_color(regs.obp + i, value);
        }
        break;
    case 0x49:
        regs.dmgp[2] = value;
        write_old_color(regs.obp + 8, value);
        break;
    case 0x4A:
        regs.wy = value;
        break;
    case 0x4B:
        regs.wx = value;
        break;
    case 0x4F:
        vram.set_bank(value);
        break;
    case 0x68:
        regs.bgpi = value;
        break;
    case 0x69:
        regs.bgp[regs.bgpi & 0x3F] = value;
        if (regs.bgpi & 0x80) {
            (++regs.bgpi &= 0x3F) |= 0x80;
        }
        break;
    case 0x6A:
        regs.obpi = value;
        break;
    case 0x6B:
        regs.obp[regs.obpi & 0x3F] = value;
        if (regs.obpi & 0x80) {
            (++regs.obpi &= 0x3F) |= 0x80;
        }
        break;
    }
}
