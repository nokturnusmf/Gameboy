#include "display.h"

#include "interrupts.h"

static const int ModeCycles[] = { 80, 172, 204 };
static const int SCANLINE = ModeCycles[0] + ModeCycles[1] + ModeCycles[2];

static const byte STAT_MASK = 0xFC;
static const byte LYC_IE = 0x40;
static const byte LYC_COINC = 0x04;

void Display::advance(long cycles) {
    cycle_count += cycles;
    switch (mode) {
    case VideoMode::Oam:
        if (cycle_count >= ModeCycles[mode]) {
            cycle_count %= ModeCycles[mode];
            regs.stat = (regs.stat & STAT_MASK) | 0b11;
            mode = VideoMode::Transfer;
        }
        break;

    case VideoMode::Transfer:
        if (cycle_count >= ModeCycles[mode]) {
            cycle_count %= ModeCycles[mode];
            mode = VideoMode::HBlank;
            regs.stat = (regs.stat & STAT_MASK) | 0b00;

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

            // draw line

            if (++regs.ly < 144) {
                mode = VideoMode::Oam;
                regs.stat = (regs.stat & STAT_MASK) | 0b10;
            } else {
                mode = VideoMode::VBlank;
                regs.stat = (regs.stat & STAT_MASK) | 0b01;
                interrupts.flags |= Interrupts::VBlank;
            }
        }
        break;

    case VideoMode::VBlank:
        if (cycle_count >= SCANLINE) {
            cycle_count %= SCANLINE;

            if (++regs.ly == 154) {
                // draw sprites
                display_callback(reinterpret_cast<byte*>(framebuffer));

                regs.ly = 0;
                mode = VideoMode::Oam;
                regs.stat = (regs.stat & STAT_MASK) | 0b10;
            }
        }
        break;
    }
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
