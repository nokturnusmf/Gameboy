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

byte& MemoryMap::operator()(word address) {
    if (address < 0x4000) {
        return rom[address];
    } else if (address < 0x8000) {
        return rom[rom_bank + address];
    } else if (address < 0xA000) {
        return vram[vram_bank + address];
    } else if (address < 0xC000) {
        throw address; // TODO external ram
    } else if (address < 0xD000) {
        return ram[address - 0xD000];
    } else if (address < 0xE000) {
        return ram[ram_bank + address];
    } else if (address < 0xFE00) {
        return (*this)(address - 0x2000);
    } else if (address < 0xFEA0) {
        throw address; // TODO sprite attribute table
    } else if (address < 0xFF00) {
        throw address; // not usable
    } else if (address < 0xFF80) {
        throw address; // TODO I/O ports
    } else if (address < 0xFFFF) {
        return hram[address - 0xFF80];
    } else {
        throw address; // TODO interrupt enable register
    }
}
