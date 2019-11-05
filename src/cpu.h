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
