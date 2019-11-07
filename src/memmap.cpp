#include "memmap.h"

#include "display.h"
#include "interrupts.h"
#include "input.h"

MemoryMap::MemoryMap(Display& display, Interrupts& interrupts, Input& input, const std::string& file_path)
    : display(display), interrupts(interrupts), input(input), ram(0x8000), vram(0x4000), oam(160), hram(127), rom(file_path) {

}

byte MemoryMap::read(word address) {
    if (address < 0x8000) {
        return rom.read(address);
    } else if (address < 0xA000) {
        return vram[address];
    } else if (address < 0xC000) {
        return rom.read(address);
    } else if (address < 0xE000) {
        return ram[address];
    } else if (address < 0xFE00) {
        return read(address - 0x2000);
    } else if (address < 0xFEA0) {
        return oam[address - 0xFE00];
    } else if (address < 0xFF00) {
        throw address; // unmapped
    } else if (address < 0xFF80) {
        return read_ctrl(address);
    } else if (address < 0xFFFF) {
        return hram[address - 0xFF80];
    } else {
        return interrupts.enabled;
    }
}

word MemoryMap::read_word(word address) {
    return (word)read(address) | (word)read(address + 1) << 8;
}

void MemoryMap::write(word address, byte value) {
    if (address < 0x8000) {
        rom.write(address, value);
    } else if (address < 0xA000) {
        vram[address] = value;
    } else if (address < 0xC000) {
        rom.write(address, value);
    } else if (address < 0xE000) {
        ram[address] = value;
    } else if (address < 0xFE00) {
        write(address - 0x2000, value);
    } else if (address < 0xFEA0) {
        oam[address - 0xFE00] = value;
    } else if (address < 0xFF00) {
        throw address; // unmapped
    } else if (address < 0xFF80) {
        write_ctrl(address, value);
    } else if (address < 0xFFFF) {
        hram[address - 0xFF80] = value;
    } else {
        interrupts.enabled = value;
    }
}

void MemoryMap::write_word(word address, word value) {
    write(address, value);
    write(address + 1, value >> 8);
}

#include <iostream>
#include <iomanip>

byte MemoryMap::read_ctrl(word address) {
    switch (address & 0xFF) {
    case 0x00:
        return input.read();

    case 0x0F:
        return interrupts.flags;

    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x4A:
    case 0x4B:
    case 0x68:
    case 0x69:
    case 0x6A:
    case 0x6B:
        return display.read_io(address);

    default:
        std::cout << "read ctrl " << std::hex << address << '\n';
        return 0;
        // throw address;
    }
}

void MemoryMap::write_ctrl(word address, byte value) {
    switch (address & 0xFF) {
    case 0x00:
        input.write(value);
        break;

    case 0x0F:
        interrupts.flags = value;
        break;

    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x4A:
    case 0x4B:
    case 0x68:
    case 0x69:
    case 0x6A:
    case 0x6B:
        display.write_io(address, value);
        break;

    case 0x46: {
        word source = (word)value << 8;
        for (int i = 0; i < 160; ++i) {
            oam[i] = read(source + i);
        }
        break;
    }

    case 0x4F:
        vram.set_bank(value);
        break;

    default:
        std::cout << "write ctrl " << std::hex << address << ' ' << (int)value << '\n';
        break;
        // throw address;
    }
}
