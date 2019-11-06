#include "memmap.h"

MemoryMap::MemoryMap(CPU& cpu, Input& input, const std::string& file_path)
    : cpu(cpu), input(input), ram(0x8000), vram(0x4000), oam(160), hram(127), rom(file_path) {

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
        return cpu.interrupts;
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
        cpu.interrupts = value;
    }
}

void MemoryMap::write_word(word address, word value) {
    write(address, value);
    write(address + 1, value >> 8);
}

#include <iostream>

byte MemoryMap::read_ctrl(word address) {
    switch (address) {
    case 0xFF00:
        return input.read();

    default:
        std::cout << "read ctrl " << address << '\n';
        return 145;
        // throw address;
    }
}

void MemoryMap::write_ctrl(word address, byte value) {
    switch (address) {
    case 0xFF00:
        input.write(value);
        return;

    default:
        std::cout << "write ctrl " << address << ' ' << (int)value << '\n';
        return;
        // throw address;
    }
}
