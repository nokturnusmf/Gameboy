#pragma once

long Emulator::execute_bits() {
    byte op = memmap.read((++cpu.pc)++);

    std::cout << "bits: " << std::hex << (int)op << '\n';

    switch (op) {
    case 0x40: // bit 0, b
        cpu.flags = (~cpu.b & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x41: // bit 0, c
        cpu.flags = (~cpu.c & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x42: // bit 0, d
        cpu.flags = (~cpu.d & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x43: // bit 0, e
        cpu.flags = (~cpu.e & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x44: // bit 0, h
        cpu.flags = (~cpu.h & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x45: // bit 0, l
        cpu.flags = (~cpu.l & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x46: // bit 0, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x47: // bit 0, a
        cpu.flags = (~cpu.a & 0x01) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x48: // bit 1, b
        cpu.flags = (~cpu.b & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x49: // bit 1, c
        cpu.flags = (~cpu.c & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x4A: // bit 1, d
        cpu.flags = (~cpu.d & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x4B: // bit 1, e
        cpu.flags = (~cpu.e & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x4C: // bit 1, h
        cpu.flags = (~cpu.h & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x4D: // bit 1, l
        cpu.flags = (~cpu.l & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x4E: // bit 1, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x4F: // bit 1, a
        cpu.flags = (~cpu.a & 0x02) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x50: // bit 2, b
        cpu.flags = (~cpu.b & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x51: // bit 2, c
        cpu.flags = (~cpu.c & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x52: // bit 2, d
        cpu.flags = (~cpu.d & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x53: // bit 2, e
        cpu.flags = (~cpu.e & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x54: // bit 2, h
        cpu.flags = (~cpu.h & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x55: // bit 2, l
        cpu.flags = (~cpu.l & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x56: // bit 2, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x57: // bit 2, a
        cpu.flags = (~cpu.a & 0x04) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x58: // bit 3, b
        cpu.flags = (~cpu.b & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x59: // bit 3, c
        cpu.flags = (~cpu.c & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x5A: // bit 3, d
        cpu.flags = (~cpu.d & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x5B: // bit 3, e
        cpu.flags = (~cpu.e & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x5C: // bit 3, h
        cpu.flags = (~cpu.h & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x5D: // bit 3, l
        cpu.flags = (~cpu.l & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x5E: // bit 3, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x5F: // bit 3, a
        cpu.flags = (~cpu.a & 0x08) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x60: // bit 4, b
        cpu.flags = (~cpu.b & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x61: // bit 4, c
        cpu.flags = (~cpu.c & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x62: // bit 4, d
        cpu.flags = (~cpu.d & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x63: // bit 4, e
        cpu.flags = (~cpu.e & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x64: // bit 4, h
        cpu.flags = (~cpu.h & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x65: // bit 4, l
        cpu.flags = (~cpu.l & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x66: // bit 4, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x67: // bit 4, a
        cpu.flags = (~cpu.a & 0x10) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x68: // bit 5, b
        cpu.flags = (~cpu.b & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x69: // bit 5, c
        cpu.flags = (~cpu.c & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x6A: // bit 5, d
        cpu.flags = (~cpu.d & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x6B: // bit 5, e
        cpu.flags = (~cpu.e & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x6C: // bit 5, h
        cpu.flags = (~cpu.h & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x6D: // bit 5, l
        cpu.flags = (~cpu.l & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x6E: // bit 5, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x6F: // bit 5, a
        cpu.flags = (~cpu.a & 0x20) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x70: // bit 6, b
        cpu.flags = (~cpu.b & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x71: // bit 6, c
        cpu.flags = (~cpu.c & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x72: // bit 6, d
        cpu.flags = (~cpu.d & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x73: // bit 6, e
        cpu.flags = (~cpu.e & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x74: // bit 6, h
        cpu.flags = (~cpu.h & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x75: // bit 6, l
        cpu.flags = (~cpu.l & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x76: // bit 6, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x77: // bit 6, a
        cpu.flags = (~cpu.a & 0x40) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x78: // bit 7, b
        cpu.flags = (~cpu.b & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x79: // bit 7, c
        cpu.flags = (~cpu.c & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x7A: // bit 7, d
        cpu.flags = (~cpu.d & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x7B: // bit 7, e
        cpu.flags = (~cpu.e & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x7C: // bit 7, h
        cpu.flags = (~cpu.h & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x7D: // bit 7, l
        cpu.flags = (~cpu.l & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x7E: // bit 7, (hl)
        cpu.flags = (~memmap.read(cpu.hl) & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 12;

    case 0x7F: // bit 7, a
        cpu.flags = (~cpu.a & 0x80) | HALFCARRY_FLAG | (cpu.flags & CARRY_FLAG);
        return 8;

    case 0x80: // res 0, b
        cpu.b &= ~0x01;
        return 8;

    case 0x81: // res 0, c
        cpu.c &= ~0x01;
        return 8;

    case 0x82: // res 0, d
        cpu.d &= ~0x01;
        return 8;

    case 0x83: // res 0, e
        cpu.e &= ~0x01;
        return 8;

    case 0x84: // res 0, h
        cpu.h &= ~0x01;
        return 8;

    case 0x85: // res 0, l
        cpu.l &= ~0x01;
        return 8;

    case 0x86: // res 0, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x01);
        return 16;

    case 0x87: // res 0, a
        cpu.a &= ~0x01;
        return 8;

    case 0x88: // res 1, b
        cpu.b &= ~0x02;
        return 8;

    case 0x89: // res 1, c
        cpu.c &= ~0x02;
        return 8;

    case 0x8A: // res 1, d
        cpu.d &= ~0x02;
        return 8;

    case 0x8B: // res 1, e
        cpu.e &= ~0x02;
        return 8;

    case 0x8C: // res 1, h
        cpu.h &= ~0x02;
        return 8;

    case 0x8D: // res 1, l
        cpu.l &= ~0x02;
        return 8;

    case 0x8E: // res 1, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x02);
        return 16;

    case 0x8F: // res 1, a
        cpu.a &= ~0x02;
        return 8;

    case 0x90: // res 2, b
        cpu.b &= ~0x04;
        return 8;

    case 0x91: // res 2, c
        cpu.c &= ~0x04;
        return 8;

    case 0x92: // res 2, d
        cpu.d &= ~0x04;
        return 8;

    case 0x93: // res 2, e
        cpu.e &= ~0x04;
        return 8;

    case 0x94: // res 2, h
        cpu.h &= ~0x04;
        return 8;

    case 0x95: // res 2, l
        cpu.l &= ~0x04;
        return 8;

    case 0x96: // res 2, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x04);
        return 16;

    case 0x97: // res 2, a
        cpu.a &= ~0x04;
        return 8;

    case 0x98: // res 3, b
        cpu.b &= ~0x08;
        return 8;

    case 0x99: // res 3, c
        cpu.c &= ~0x08;
        return 8;

    case 0x9A: // res 3, d
        cpu.d &= ~0x08;
        return 8;

    case 0x9B: // res 3, e
        cpu.e &= ~0x08;
        return 8;

    case 0x9C: // res 3, h
        cpu.h &= ~0x08;
        return 8;

    case 0x9D: // res 3, l
        cpu.l &= ~0x08;
        return 8;

    case 0x9E: // res 3, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x08);
        return 16;

    case 0x9F: // res 3, a
        cpu.a &= ~0x08;
        return 8;

    case 0xA0: // res 4, b
        cpu.b &= ~0x10;
        return 8;

    case 0xA1: // res 4, c
        cpu.c &= ~0x10;
        return 8;

    case 0xA2: // res 4, d
        cpu.d &= ~0x10;
        return 8;

    case 0xA3: // res 4, e
        cpu.e &= ~0x10;
        return 8;

    case 0xA4: // res 4, h
        cpu.h &= ~0x10;
        return 8;

    case 0xA5: // res 4, l
        cpu.l &= ~0x10;
        return 8;

    case 0xA6: // res 4, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x10);
        return 16;

    case 0xA7: // res 4, a
        cpu.a &= ~0x10;
        return 8;

    case 0xA8: // res 5, b
        cpu.b &= ~0x20;
        return 8;

    case 0xA9: // res 5, c
        cpu.c &= ~0x20;
        return 8;

    case 0xAA: // res 5, d
        cpu.d &= ~0x20;
        return 8;

    case 0xAB: // res 5, e
        cpu.e &= ~0x20;
        return 8;

    case 0xAC: // res 5, h
        cpu.h &= ~0x20;
        return 8;

    case 0xAD: // res 5, l
        cpu.l &= ~0x20;
        return 8;

    case 0xAE: // res 5, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x20);
        return 16;

    case 0xAF: // res 5, a
        cpu.a &= ~0x20;
        return 8;

    case 0xB0: // res 6, b
        cpu.b &= ~0x40;
        return 8;

    case 0xB1: // res 6, c
        cpu.c &= ~0x40;
        return 8;

    case 0xB2: // res 6, d
        cpu.d &= ~0x40;
        return 8;

    case 0xB3: // res 6, e
        cpu.e &= ~0x40;
        return 8;

    case 0xB4: // res 6, h
        cpu.h &= ~0x40;
        return 8;

    case 0xB5: // res 6, l
        cpu.l &= ~0x40;
        return 8;

    case 0xB6: // res 6, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x40);
        return 16;

    case 0xB7: // res 6, a
        cpu.a &= ~0x40;
        return 8;

    case 0xB8: // res 7, b
        cpu.b &= ~0x80;
        return 8;

    case 0xB9: // res 7, c
        cpu.c &= ~0x80;
        return 8;

    case 0xBA: // res 7, d
        cpu.d &= ~0x80;
        return 8;

    case 0xBB: // res 7, e
        cpu.e &= ~0x80;
        return 8;

    case 0xBC: // res 7, h
        cpu.h &= ~0x80;
        return 8;

    case 0xBD: // res 7, l
        cpu.l &= ~0x80;
        return 8;

    case 0xBE: // res 7, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) & ~0x80);
        return 16;

    case 0xBF: // res 7, a
        cpu.a &= ~0x80;
        return 8;

    case 0xC0: // set 0, b
        cpu.b |= 0x01;
        return 8;

    case 0xC1: // set 0, c
        cpu.c |= 0x01;
        return 8;

    case 0xC2: // set 0, d
        cpu.d |= 0x01;
        return 8;

    case 0xC3: // set 0, e
        cpu.e |= 0x01;
        return 8;

    case 0xC4: // set 0, h
        cpu.h |= 0x01;
        return 8;

    case 0xC5: // set 0, l
        cpu.l |= 0x01;
        return 8;

    case 0xC6: // set 0, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x01);
        return 16;

    case 0xC7: // set 0, a
        cpu.a |= 0x01;
        return 8;

    case 0xC8: // set 1, b
        cpu.b |= 0x02;
        return 8;

    case 0xC9: // set 1, c
        cpu.c |= 0x02;
        return 8;

    case 0xCA: // set 1, d
        cpu.d |= 0x02;
        return 8;

    case 0xCB: // set 1, e
        cpu.e |= 0x02;
        return 8;

    case 0xCC: // set 1, h
        cpu.h |= 0x02;
        return 8;

    case 0xCD: // set 1, l
        cpu.l |= 0x02;
        return 8;

    case 0xCE: // set 1, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x02);
        return 16;

    case 0xCF: // set 1, a
        cpu.a |= 0x02;
        return 8;

    case 0xD0: // set 2, b
        cpu.b |= 0x04;
        return 8;

    case 0xD1: // set 2, c
        cpu.c |= 0x04;
        return 8;

    case 0xD2: // set 2, d
        cpu.d |= 0x04;
        return 8;

    case 0xD3: // set 2, e
        cpu.e |= 0x04;
        return 8;

    case 0xD4: // set 2, h
        cpu.h |= 0x04;
        return 8;

    case 0xD5: // set 2, l
        cpu.l |= 0x04;
        return 8;

    case 0xD6: // set 2, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x04);
        return 16;

    case 0xD7: // set 2, a
        cpu.a |= 0x04;
        return 8;

    case 0xD8: // set 3, b
        cpu.b |= 0x08;
        return 8;

    case 0xD9: // set 3, c
        cpu.c |= 0x08;
        return 8;

    case 0xDA: // set 3, d
        cpu.d |= 0x08;
        return 8;

    case 0xDB: // set 3, e
        cpu.e |= 0x08;
        return 8;

    case 0xDC: // set 3, h
        cpu.h |= 0x08;
        return 8;

    case 0xDD: // set 3, l
        cpu.l |= 0x08;
        return 8;

    case 0xDE: // set 3, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x08);
        return 16;

    case 0xDF: // set 3, a
        cpu.a |= 0x08;
        return 8;

    case 0xE0: // set 4, b
        cpu.b |= 0x10;
        return 8;

    case 0xE1: // set 4, c
        cpu.c |= 0x10;
        return 8;

    case 0xE2: // set 4, d
        cpu.d |= 0x10;
        return 8;

    case 0xE3: // set 4, e
        cpu.e |= 0x10;
        return 8;

    case 0xE4: // set 4, h
        cpu.h |= 0x10;
        return 8;

    case 0xE5: // set 4, l
        cpu.l |= 0x10;
        return 8;

    case 0xE6: // set 4, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x10);
        return 16;

    case 0xE7: // set 4, a
        cpu.a |= 0x10;
        return 8;

    case 0xE8: // set 5, b
        cpu.b |= 0x20;
        return 8;

    case 0xE9: // set 5, c
        cpu.c |= 0x20;
        return 8;

    case 0xEA: // set 5, d
        cpu.d |= 0x20;
        return 8;

    case 0xEB: // set 5, e
        cpu.e |= 0x20;
        return 8;

    case 0xEC: // set 5, h
        cpu.h |= 0x20;
        return 8;

    case 0xED: // set 5, l
        cpu.l |= 0x20;
        return 8;

    case 0xEE: // set 5, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x20);
        return 16;

    case 0xEF: // set 5, a
        cpu.a |= 0x20;
        return 8;

    case 0xF0: // set 6, b
        cpu.b |= 0x40;
        return 8;

    case 0xF1: // set 6, c
        cpu.c |= 0x40;
        return 8;

    case 0xF2: // set 6, d
        cpu.d |= 0x40;
        return 8;

    case 0xF3: // set 6, e
        cpu.e |= 0x40;
        return 8;

    case 0xF4: // set 6, h
        cpu.h |= 0x40;
        return 8;

    case 0xF5: // set 6, l
        cpu.l |= 0x40;
        return 8;

    case 0xF6: // set 6, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x40);
        return 16;

    case 0xF7: // set 6, a
        cpu.a |= 0x40;
        return 8;

    case 0xF8: // set 7, b
        cpu.b |= 0x80;
        return 8;

    case 0xF9: // set 7, c
        cpu.c |= 0x80;
        return 8;

    case 0xFA: // set 7, d
        cpu.d |= 0x80;
        return 8;

    case 0xFB: // set 7, e
        cpu.e |= 0x80;
        return 8;

    case 0xFC: // set 7, h
        cpu.h |= 0x80;
        return 8;

    case 0xFD: // set 7, l
        cpu.l |= 0x80;
        return 8;

    case 0xFE: // set 7, (hl)
        memmap.write(cpu.hl, memmap.read(cpu.hl) | 0x80);
        return 16;

    case 0xFF: // set 7, a
        cpu.a |= 0x80;
        return 8;

    default:
        std::cerr << "bits unimplemented " << std::hex << (int)op << '\n';
        { char c; std::cin >> c; }
        return 4;
    }
}
