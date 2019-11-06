#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "cpu.h"
#include "rom.h"

class MemoryMap {
public:
    MemoryMap(CPU& cpu, const std::string& file_path);

    byte* physical(word address);

    byte read(word address);
    word read_word(word address);

    void write(word address, byte value);
    void write_word(word address, word value);

private:
    byte read_ctrl(word address);
    void write_ctrl(word address, byte value);

    CPU& cpu;

    std::vector<byte> ram;
    std::vector<byte> vram;
    std::vector<byte> hram;
    std::vector<byte> xram;

    Rom rom;

    int ram_bank;
    int vram_bank;
};
