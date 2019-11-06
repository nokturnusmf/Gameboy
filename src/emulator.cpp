#include "emulator.h"

#include <iostream>

Emulator::Emulator(const std::string& file_path)
    : cpu({ }), memmap(file_path) {
    cpu.a = 0x11;
    cpu.sp = 0xFFFE;
    cpu.pc = 0x100;
}

void Emulator::run() {
    long cycles = 0;
    while (true) {
        cycles += execute();
    }
}

#include "opcodes.h"

#include "opcodes_bits.h"
