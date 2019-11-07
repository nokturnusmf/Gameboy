#include "display.h"

void Display::advance(long cycles) {
    if ((cycle_count += cycles) >= 9198) {
        cycle_count -= 9198;
        if (++ly > 153) {
            display_callback(framebuffer);
            ly = 0;
        }
    }
}

#include <iostream>
#include <iomanip>

byte Display::read_io(word address) {
    if (address == 0xFF44) return ly;
    std::cout << "lcd read " << std::hex << address << '\n';
    switch (address & 0xFF) {
    case 0x40:
        return lcdc;

    case 0x41:
        return stat;

    case 0x42:
        return scy;

    case 0x43:
        return scx;

    case 0x44:
        return ly;

    case 0x45:
        return lyc;

    case 0x4A:
        return wy;

    case 0x4B:
        return wx;

    case 0x68:
        return bgpi;

    case 0x69:
        return bgpd;

    case 0x6A:
        return obpi;

    case 0x6B:
        return obpd;

    default:
        std::cerr << "lcd invalid read " << address << '\n';
        return 0;
    }
}

void Display::write_io(word address, byte value) {
    std::cout << "lcd write " << std::hex << address << ' ' << (int)value << '\n';
    switch (address & 0xFF) {
    case 0x40:
        lcdc = value;
        break;

    case 0x41:
        stat = value;
        break;

    case 0x42:
        scy = value;
        break;

    case 0x43:
        scx = value;
        break;

    case 0x44:
        ly = value;
        break;

    case 0x45:
        lyc = value;
        break;

    case 0x4A:
        wy = value;
        break;

    case 0x4B:
        wx = value;
        break;

    case 0x68:
        bgpi = value;
        break;

    case 0x69:
        bgpd = value;
        break;

    case 0x6A:
        obpi = value;
        break;

    case 0x6B:
        obpd = value;
        break;

    default:
        std::cerr << "lcd invalid write " << address << ' ' << (int)value << '\n';
    }
}
