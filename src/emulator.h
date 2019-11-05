#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "cpu.h"
#include "memmap.h"

class Emulator {
public:
    Emulator(const std::string& file_path);
    // ~Emulator();

    void run();

    long execute();

private:
    CPU cpu;
    MemoryMap memmap;
};
