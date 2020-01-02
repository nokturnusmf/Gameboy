#include "memmap.h"

#include "display.h"
#include "interrupts.h"
#include "input.h"

MemoryMap::MemoryMap(Display& display, Interrupts& interrupts, Timer& timer, Input& input, const std::string& file_path)
    : display(display), interrupts(interrupts), timer(timer), input(input), ram(0x8000), oam(160), hram(127), rom(file_path) {
}

byte MemoryMap::read(word address) {
    if (address < 0x8000) {
        return rom.read(address);
    } else if (address < 0xA000) {
        return display.read(address);
    } else if (address < 0xC000) {
        return rom.read(address);
    } else if (address < 0xE000) {
        return ram[address];
    } else if (address < 0xFE00) {
        return read(address - 0x2000);
    } else if (address < 0xFEA0) {
        return oam[address - 0xFE00];
    } else if (address < 0xFF00) {
        return 0xFF; // invalid address
    } else if (address < 0xFF80) {
        return read_ctrl(address);
    } else if (address < 0xFFFF) {
        return hram[address - 0xFF80];
    } else {
        return interrupts.enabled;
    }
}

word MemoryMap::read_word(word address) {
    return (word)read(address) | (word)read(address + 1) << 8;
}

void MemoryMap::write(word address, byte value) {
    if (address < 0x8000) {
        rom.write(address, value);
    } else if (address < 0xA000) {
        display.write(address, value);
    } else if (address < 0xC000) {
        rom.write(address, value);
    } else if (address < 0xE000) {
        ram[address] = value;
    } else if (address < 0xFE00) {
        write(address - 0x2000, value);
    } else if (address < 0xFEA0) {
        oam[address - 0xFE00] = value;
    } else if (address < 0xFF00) {
        // ignored
    } else if (address < 0xFF80) {
        write_ctrl(address, value);
    } else if (address < 0xFFFF) {
        hram[address - 0xFF80] = value;
    } else {
        interrupts.enabled = value;
    }
}

void MemoryMap::write_word(word address, word value) {
    write(address, value);
    write(address + 1, value >> 8);
}

byte MemoryMap::read_ctrl(word address) {
    switch (address & 0xFF) {
    case 0x00:
        return input.read();
    case 0x04:
        return timer.div;
    case 0x05:
        return timer.tima;
    case 0x06:
        return timer.tma;
    case 0x07:
        return timer.tac;
    case 0x0F:
        return interrupts.flags;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4F:
    case 0x68:
    case 0x69:
    case 0x6A:
    case 0x6B:
        return display.read_io(address & 0xFF);

    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
        // TODO hdma read
        return 0;

    case 0x70:
        return ram.get_bank();

    default:
        return 0;
    }
}

void MemoryMap::write_ctrl(word address, byte value) {
    switch (address & 0xFF) {
    case 0x00:
        input.write(value);
        break;
    case 0x04:
        timer.div = 0;
        break;
    case 0x05:
        timer.tima = value;
        break;
    case 0x06:
        timer.tma = value;
        break;
    case 0x07:
        timer.tac = value;
        break;
    case 0x0F:
        interrupts.flags = value;
        break;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4F:
    case 0x68:
    case 0x69:
    case 0x6A:
    case 0x6B:
        display.write_io(address & 0xFF, value);
        break;
    case 0x46: {
        word source = (word)value << 8;
        for (int i = 0; i < 160; ++i) {
            oam[i] = read(source + i);
        }
        break;
    }

    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
        // TODO hmda write
        break;

    case 0x70:
        ram.set_bank(value ? value : 1);
        return;

    default:
        break;
    }
}
