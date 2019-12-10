#pragma once

#include <vector>
#include <string>

#include "types.h"
#include "cpu.h"
#include "memmap.h"
#include "interrupts.h"
#include "display.h"
#include "timer.h"
#include "input.h"

class Emulator {
public:
    Emulator(const std::string& file_path, bool(*display_callback)(byte*));

    Input& get_input() { return input; }

    void run();

private:
    int execute();
    int execute_bits();

    CPU cpu;
    MemoryMap memmap;
    Interrupts interrupts;
    Display display;
    Timer timer;
    Input input;
};
