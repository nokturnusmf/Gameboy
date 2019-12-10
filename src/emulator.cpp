#include "emulator.h"

Emulator::Emulator(const std::string& file_path, bool(*display_callback)(byte*))
    : cpu({ }), memmap(display, interrupts, timer, input, file_path), interrupts(cpu, memmap),
      display(memmap, interrupts, display_callback), timer(interrupts) {
}

void Emulator::run() {
    while (!display.close_requested()) {
        int cycles = interrupts.process() + execute();
        display.advance(cycles);
        timer.update(cycles);
    }
}

#include "opcodes.impl"

#include "opcodes_bits.impl"
