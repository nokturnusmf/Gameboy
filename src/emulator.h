#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "cpu.h"
#include "memmap.h"
#include "interrupts.h"
#include "display.h"
#include "input.h"

class Emulator {
public:
    Emulator(const std::string& file_path);

    void run();

    long execute();
    long execute_bits();

private:
    CPU cpu;
    MemoryMap memmap;
    Interrupts interrupts;
    Display display;
    Input input;
};
