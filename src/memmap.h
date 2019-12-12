#pragma once

#include <vector>
#include <string>

#include "types.h"
#include "bank.h"
#include "rom.h"
#include "timer.h"

struct Interrupts;
class Display;
class Input;

class MemoryMap {
public:
    MemoryMap(Display& display, Interrupts& interrupts, Timer& timer, Input& input, const std::string& file_path);

    byte read(word address);
    word read_word(word address);

    void write(word address, byte value);
    void write_word(word address, word value);

private:
    byte read_ctrl(word address);
    void write_ctrl(word address, byte value);

    Display& display;
    Interrupts& interrupts;
    Timer& timer;
    Input& input;

    BankedMemory<0xC000, 0x1000, 0x1000> ram;

    std::vector<byte> oam;
    std::vector<byte> hram;

    Rom rom;

    bool boot = true;
};
