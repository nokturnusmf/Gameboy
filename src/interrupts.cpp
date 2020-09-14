#include "interrupts.h"

#include "cpu.h"
#include "memmap.h"

static void do_call(CPU* cpu, MemoryMap* memmap, word address) {
    memmap->write_word(cpu->sp -= 2, cpu->pc);
    cpu->pc = address;
}

int Interrupts::process() {
    byte b = enabled & flags;

    if (!ime) {
        if (b) {
            cpu->halt = false;
            return 4;
        } else {
            return 0;
        }
    }

    b &= -b;
    switch (b) {
    case 0:
        return 0;

    case Interrupts::VBlank:
        do_call(cpu, memmap, 0x40);
        break;

    case Interrupts::LCDStat:
        do_call(cpu, memmap, 0x48);
        break;

    case Interrupts::Timer:
        do_call(cpu, memmap, 0x50);
        break;

    case Interrupts::Serial:
        do_call(cpu, memmap, 0x58);
        break;

    case Interrupts::Joypad:
        do_call(cpu, memmap, 0x60);
        break;
    }

    ime = false;
    flags &= ~b;
    long cycles = cpu->halt ? 24 : 20;
    cpu->halt = false;
    return cycles;
}
