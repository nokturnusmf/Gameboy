#include "emulator.h"

#include <iostream>

Emulator::Emulator(const std::string& file_path)
    : cpu({ }), memmap(interrupts, input, file_path), interrupts(cpu, memmap) {
    cpu.a = 0x11;
    cpu.sp = 0xFFFE;
    cpu.pc = 0x100;
}

void Emulator::run() {
    long cycles = 0;
    while (true) {
        cycles += execute();
        interrupts.process();
    }
}

#include "opcodes.impl"

#include "opcodes_bits.impl"
