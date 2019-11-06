#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "cpu.h"

class MemoryMap {
public:
    MemoryMap(const std::string& file_path);

    byte* physical(word address);

    byte read(word address);
    word read_word(word address);

    void write(word address, byte value);
    void write_word(word address, word value);

private:
    byte read_ctrl(word address);
    void write_ctrl(word address, byte value);

    std::vector<byte> ram;
    std::vector<byte> vram;
    std::vector<byte> hram;
    std::vector<byte> xram;

    std::vector<byte> rom;

    int ram_bank;
    int vram_bank;
    int rom_bank;
};
