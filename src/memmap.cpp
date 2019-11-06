#include "memmap.h"

#include <fstream>

static std::vector<byte> load_rom(const std::string& file_path) {
    std::vector<byte> rom(0x200000);
    std::ifstream file(file_path);
    if (!file) {
        throw; // TODO
    }
    file.read(reinterpret_cast<char*>(&rom[0]), 0x200000);
    return rom;
}

MemoryMap::MemoryMap(const std::string& file_path)
    : ram(0x8000), vram(0x4000), hram(127), rom(load_rom(file_path)), ram_bank(0x1000 - 0xD000), vram_bank(-0x8000), rom_bank(0) {

}

byte* MemoryMap::physical(word address) {
    if (address < 0x4000) {
        return &rom[address];
    } else if (address < 0x8000) {
        return &rom[rom_bank + address];
    } else if (address < 0xA000) {
        return &vram[vram_bank + address];
    } else if (address < 0xC000) {
        throw address; // TODO external ram
    } else if (address < 0xD000) {
        return &ram[address - 0xD000];
    } else if (address < 0xE000) {
        return &ram[ram_bank + address];
    } else if (address < 0xFE00) {
        return physical(address - 0x2000);
    } else if (address < 0xFEA0) {
        throw address; // TODO sprite attribute table
    } else if (address < 0xFF00) {
        throw address; // not usable
    } else if (address < 0xFF80) {
        throw address; // control registers
    } else if (address < 0xFFFF) {
        return &hram[address - 0xFF80];
    } else {
        throw address; // control register
    }
}

byte MemoryMap::read(word address) {
    if (address < 0xFF00) {
        return *physical(address);
    } else if (address < 0xFF80) {
        return read_ctrl(address);
    } else if (address < 0xFFFF) {
        return hram[address - 0xFF80];
    } else {
        // return cpu.interrupts;
        return read_ctrl(address);
    }
}

word MemoryMap::read_word(word address) {
    return *reinterpret_cast<word*>(physical(address));
}

void MemoryMap::write(word address, byte value) {
    if (address < 0x8000) {
        // rom bank switching
        throw address;
    } else if (address < 0xFF00) {
        *physical(address) = value;
    } else if (address < 0xFF80) {
        write_ctrl(address, value);
    } else if (address < 0xFFFF) {
        hram[address - 0xFF80] = value;
    } else {
        // cpu.interrupts = value;
        write_ctrl(address, value);
    }
}

void MemoryMap::write_word(word address, word value) {
    *reinterpret_cast<word*>(physical(address)) = value;
}

byte MemoryMap::read_ctrl(word address) {
    // throw address;
    return 0;
}

void MemoryMap::write_ctrl(word address, byte value) {
    // throw address;
}
