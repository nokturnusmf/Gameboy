#pragma once

#include "gb.h"

struct CPU {
    union {
        struct { byte flags; byte a; };
        word af;
    };
    union {
        struct { byte c; byte b; };
        word bc;
    };
    union {
        struct { byte e; byte d; };
        word de;
    };
    union {
        struct { byte l; byte h; };
        word hl;
    };
    word sp;
    word pc;
};

const byte ZERO_FLAG = 0x80;
const byte ADDSUB_FLAG = 0x40;
const byte HALFCARRY_FLAG = 0x20;
const byte CARRY_FLAG = 0x10;
