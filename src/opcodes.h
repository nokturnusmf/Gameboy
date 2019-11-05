#pragma once

long Emulator::execute() {
    byte op = memmap(cpu.pc);

    std::cout << std::hex << (int)cpu.pc << ": " << (int)op << '\n';

    switch (op) {
    case 0: // nop
        ++cpu.pc;
        return 4;

    case 0x18: // jr imm
        cpu.pc += (sbyte)memmap(cpu.pc + 1);
        return 12;

    case 0x20: // jr imm # if nz
        if (cpu.flags & ZERO_FLAG) {
            cpu.pc += 2;
            return 8;
        } else {
            cpu.pc += (sbyte)memmap(cpu.pc + 1);
            return 12;
        }

    case 0x28: // jr imm # if z
        if (cpu.flags & ZERO_FLAG) {
            cpu.pc += (sbyte)memmap(cpu.pc + 1);
            return 12;
        } else {
            cpu.pc += 2;
            return 8;
        }

    case 0x3E: // ld imm # into a
        cpu.a = memmap((++cpu.pc)++);
        return 8;

    case 0x40: // ld b, b
        ++cpu.pc;
        return 4;

    case 0x41: // ld b, c
        cpu.b = cpu.c;
        ++cpu.pc;
        return 4;

    case 0x42: // ld b, d
        cpu.b = cpu.d;
        ++cpu.pc;
        return 4;

    case 0x43: // ld b, e
        cpu.b = cpu.e;
        ++cpu.pc;
        return 4;

    case 0x44: // ld b, h
        cpu.b = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x45: // ld b, l
        cpu.b = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x46: // ld b, (hl)
        cpu.b = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x47: // ld b, a
        cpu.b = cpu.a;
        ++cpu.pc;
        return 4;

    case 0x48: // ld c, b
        cpu.c = cpu.b;
        ++cpu.pc;
        return 4;

    case 0x49: // ld c, c
        ++cpu.pc;
        return 4;

    case 0x4A: // ld c, d
        cpu.c = cpu.d;
        ++cpu.pc;
        return 4;

    case 0x4B: // ld c, e
        cpu.c = cpu.e;
        ++cpu.pc;
        return 4;

    case 0x4C: // ld c, h
        cpu.c = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x4D: // ld c, l
        cpu.c = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x4E: // ld c, (hl)
        cpu.c = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x4F: // ld c, a
        cpu.c = cpu.a;
        ++cpu.pc;
        return 4;

    case 0x50: // ld d, b
        cpu.d = cpu.b;
        ++cpu.pc;
        return 4;

    case 0x51: // ld d, c
        cpu.d = cpu.c;
        ++cpu.pc;
        return 4;

    case 0x52: // ld d, d
        ++cpu.pc;
        return 4;

    case 0x53: // ld d, e
        cpu.d = cpu.e;
        ++cpu.pc;
        return 4;

    case 0x54: // ld d, h
        cpu.d = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x55: // ld d, l
        cpu.d = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x56: // ld d, (hl)
        cpu.d = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x57: // ld d, a
        cpu.d = cpu.a;
        ++cpu.pc;
        return 4;

    case 0x58: // ld e, b
        cpu.e = cpu.b;
        ++cpu.pc;
        return 4;

    case 0x59: // ld e, c
        cpu.e = cpu.c;
        ++cpu.pc;
        return 4;

    case 0x5A: // ld e, d
        cpu.e = cpu.d;
        ++cpu.pc;
        return 4;

    case 0x5B: // ld e, e
        ++cpu.pc;
        return 4;

    case 0x5C: // ld e, h
        cpu.e = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x5D: // ld e, l
        cpu.e = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x5E: // ld e, (hl)
        cpu.e = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x5F: // ld e, a
        cpu.e = cpu.a;
        ++cpu.pc;
        return 4;

    case 0x60: // ld h, b
        cpu.h = cpu.b;
        ++cpu.pc;
        return 4;

    case 0x61: // ld h, c
        cpu.h = cpu.c;
        ++cpu.pc;
        return 4;

    case 0x62: // ld h, d
        cpu.h = cpu.d;
        ++cpu.pc;
        return 4;

    case 0x63: // ld h, e
        cpu.h = cpu.e;
        ++cpu.pc;
        return 4;

    case 0x64: // ld h, h
        cpu.h = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x65: // ld h, l
        cpu.h = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x66: // ld h, (hl)
        cpu.h = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x67: // ld h, a
        cpu.h = cpu.a;
        ++cpu.pc;
        return 4;

    case 0x68: // ld l, b
        cpu.l = cpu.b;
        ++cpu.pc;
        return 4;

    case 0x69: // ld l, c
        cpu.l = cpu.c;
        ++cpu.pc;
        return 4;

    case 0x6A: // ld l, d
        cpu.l = cpu.d;
        ++cpu.pc;
        return 4;

    case 0x6B: // ld l, e
        cpu.l = cpu.e;
        ++cpu.pc;
        return 4;

    case 0x6C: // ld l, h
        cpu.l = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x6D: // ld l, l
        cpu.l = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x6E: // ld l, (hl)
        cpu.l = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x6F: // ld l, a
        cpu.l = cpu.a;
        ++cpu.pc;
        return 4;

    case 0x70: // ld (hl), b
        memmap(cpu.hl) = cpu.b;
        ++cpu.pc;
        return 8;

    case 0x71: // ld (hl), c
        memmap(cpu.hl) = cpu.c;
        ++cpu.pc;
        return 8;

    case 0x72: // ld (hl), d
        memmap(cpu.hl) = cpu.d;
        ++cpu.pc;
        return 8;

    case 0x73: // ld (hl), e
        memmap(cpu.hl) = cpu.e;
        ++cpu.pc;
        return 8;

    case 0x74: // ld (hl), h
        memmap(cpu.hl) = cpu.h;
        ++cpu.pc;
        return 8;

    case 0x75: // ld (hl), l
        memmap(cpu.hl) = cpu.l;
        ++cpu.pc;
        return 8;

    case 0x76: // hlt
        std::cout << "halt\n";
        return 0;

    case 0x77: // ld (hl), a
        memmap(cpu.hl) = cpu.a;
        ++cpu.pc;
        return 8;

    case 0x78: // ld a, b
        cpu.a = cpu.b;
        ++cpu.pc;
        return 4;

    case 0x79: // ld a, c
        cpu.a = cpu.c;
        ++cpu.pc;
        return 4;

    case 0x7A: // ld a, d
        cpu.a = cpu.d;
        ++cpu.pc;
        return 4;

    case 0x7B: // ld a, e
        cpu.a = cpu.e;
        ++cpu.pc;
        return 4;

    case 0x7C: // ld a, h
        cpu.a = cpu.h;
        ++cpu.pc;
        return 4;

    case 0x7D: // ld a, l
        cpu.a = cpu.l;
        ++cpu.pc;
        return 4;

    case 0x7E: // ld a, (hl)
        cpu.a = memmap(cpu.hl);
        ++cpu.pc;
        return 8;

    case 0x7F: // ld a, a
        ++cpu.pc;
        return 4;

    case 0xAF: // xor a # with a
        cpu.a = 0;
        ++cpu.pc;
        return 4;

    case 0xC3: // jmp imm
        cpu.pc = *(word*)&memmap(cpu.pc + 1);
        return 16;

    case 0xCB: // bits
        return execute_bits();

    case 0xCD: // call imm
        *(word*)&memmap((--cpu.sp)--) = cpu.pc + 3;
        cpu.pc = *(word*)&memmap(cpu.pc + 1);
        return 24;

    case 0xCF: // rst 08
        *(word*)&memmap((--cpu.sp)--) = cpu.pc + 1;
        cpu.pc = 0x08;
        return 16;

    case 0xE0: // ld io[imm] # with a
        memmap(0xFF00 + memmap((++cpu.pc)++)) = cpu.a;
        return 12;

    case 0xEA: // ld (imm) # with a
        memmap(*(word*)&memmap(++cpu.pc)) = cpu.a;
        cpu.pc += 2;
        return 16;

    case 0xF0: // ld io[imm] # into a
        cpu.a = memmap(0xFF00 + memmap((++cpu.pc)++));
        return 12;

    case 0xF3: // di
        cpu.interrupts = false;
        ++cpu.pc;
        return 4;

    case 0xFB: // ei
        cpu.interrupts = true;
        ++cpu.pc;
        return 4;

    case 0xFE: { // cmp imm # with a
        byte b = memmap((++cpu.pc)++);
        if (cpu.a == b) {
            cpu.flags = ZERO_FLAG;
        } else if (cpu.a > b) {
            cpu.flags = CARRY_FLAG;
        }
        cpu.flags |= ADDSUB_FLAG;
        // TODO HALFCARRY_FLAG
        return 8;
    }

    case 0xFF: // rst 38
        *(word*)&memmap((--cpu.sp)--) = cpu.pc + 1;
        cpu.pc = 0x38;
        return 16;

    default:
        std::cerr << "unimplemented " << std::hex << (int)op << '\n';
        { char c; std::cin >> c; }
        ++cpu.pc;
        return 4;
    }
}
