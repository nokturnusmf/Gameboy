#pragma once

#include "gb.h"

struct CPU {
    union {
        struct { byte a; byte flags; };
        word af;
    };
    union {
        struct { byte b; byte c; };
        word bc;
    };
    union {
        struct { byte d; byte e; };
        word de;
    };
    union {
        struct { byte h; byte l; };
        word hl;
    };
    word sp;
    word pc;
};

const byte ZERO_FLAG = 0x80;
const byte ADDSUB_FLAG = 0x40;
const byte HALFCARRY_FLAG = 0x20;
const byte CARRY_FLAG = 0x10;
