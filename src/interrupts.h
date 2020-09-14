#pragma once

#include "types.h"

struct CPU;
class MemoryMap;

struct Interrupts {
    Interrupts(CPU* cpu, MemoryMap* memmap) : cpu(cpu), memmap(memmap) {}

    enum {
        VBlank = 0x01,
        LCDStat = 0x02,
        Timer = 0x04,
        Serial = 0x08,
        Joypad = 0x10
    };

    int process();

    bool ime = 0;
    byte enabled = 0;
    byte flags = 0;

private:
    CPU* cpu;
    MemoryMap* memmap;
};
