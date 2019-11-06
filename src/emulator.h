#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "cpu.h"
#include "memmap.h"
#include "input.h"

class Emulator {
public:
    Emulator(const std::string& file_path);
    // ~Emulator();

    void run();

    long execute();
    long execute_bits();

private:
    CPU cpu;
    Input input;
    MemoryMap memmap;
};
