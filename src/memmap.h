#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "cpu.h"
#include "bank.h"
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

    BankedMemory<0xC000, 0x1000, 0x1000> ram;
    BankedMemory<0x8000, 0x2000, 0x0000> vram;

    std::vector<byte> hram;

    Rom rom;
};
