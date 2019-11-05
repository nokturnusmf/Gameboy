#pragma once

#include <vector>
#include <string>

#include "gb.h"

class MemoryMap {
public:
    MemoryMap(const std::string& file_path);

    byte& operator()(word address);

private:
    std::vector<byte> ram;
    std::vector<byte> vram;
    std::vector<byte> hram;

    std::vector<byte> rom;

    int ram_bank;
    int vram_bank;
    int rom_bank;
};
