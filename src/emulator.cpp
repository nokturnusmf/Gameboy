#include "emulator.h"

Emulator::Emulator(const std::string& file_path, bool(*display_callback)(byte*))
    : cpu({ }), memmap(&display, &interrupts, &timer, &input, file_path), interrupts(&cpu, &memmap),
      display(&memmap, &interrupts, display_callback), timer(&interrupts) {
    cpu.pc = 0x100;
    cpu.sp = 0xFFFE;
    cpu.a = 0x11;
}

void Emulator::run() {
    while (!display.close_requested()) {
        this->step();
    }
}

void Emulator::step() {
    int cycles = interrupts.process() + execute();
    display.advance(cycles);
    timer.update(cycles);
}

#include "opcodes.impl"

#include "opcodes_bits.impl"
