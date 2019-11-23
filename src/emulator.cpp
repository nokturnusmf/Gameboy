#include "emulator.h"

Emulator::Emulator(const std::string& file_path, bool(*display_callback)(byte*))
    : cpu({ }), memmap(display, interrupts, timer, input, file_path), interrupts(cpu, memmap),
      display(memmap, interrupts, display_callback), timer(interrupts) {
}

void Emulator::run() {
    long cycles, total_cycles = 0;
    while (!display.close_requested()) {
        total_cycles += cycles = execute();
        display.advance(cycles);
        timer.update(cycles);
        interrupts.process();
    }
}

#include "opcodes.impl"

#include "opcodes_bits.impl"
