#pragma once

#include "gb.h"

class CPU;
class MemoryMap;

struct Interrupts {
    Interrupts(CPU& cpu, MemoryMap& memmap) : cpu(cpu), memmap(memmap) {}

    void process();

    bool ime = 0;
    byte enabled = 0;
    byte flags = 0;

private:
    CPU& cpu;
    MemoryMap& memmap;
};
