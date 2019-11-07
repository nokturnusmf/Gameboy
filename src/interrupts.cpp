#include "interrupts.h"

#include "cpu.h"
#include "memmap.h"

static void do_call(CPU& cpu, MemoryMap& memmap, word address) {
    memmap.write_word((--cpu.sp)--, cpu.pc);
    cpu.pc = address;
}

void Interrupts::process() {
    if (!ime) {
        return;
    }

    byte b = enabled & flags;
    b &= -b;
    switch (b) {
    case 0:
        return;

    case 0x01:
        do_call(cpu, memmap, 0x40);
        break;

    case 0x02:
        do_call(cpu, memmap, 0x48);
        break;

    case 0x04:
        do_call(cpu, memmap, 0x50);
        break;

    case 0x08:
        do_call(cpu, memmap, 0x58);
        break;

    case 0x10:
        do_call(cpu, memmap, 0x60);
        break;
    }
    ime = false;
    flags &= ~b;
}
