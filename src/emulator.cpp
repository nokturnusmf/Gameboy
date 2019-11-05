#include "emulator.h"

#include <iostream>

Emulator::Emulator(const std::string& file_path)
    : cpu({ }), memmap(file_path) {
    cpu.sp = 0xFFFE;
    cpu.pc = 0x100;
}

void Emulator::run() {
    long cycles = 0;
    while (true) {
        cycles += execute();
    }
}

long Emulator::execute() {
    byte op = memmap(cpu.pc);
    switch (op) {

    default:
        std::cerr << "unimplemented " << std::hex << (int)op << '\n';
        { char c; std::cin >> c; }
        ++cpu.pc;
        return 4;
    }
}
