#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "bank.h"
#include "rom.h"
#include "timer.h"

class Display;
class Interrupts;
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
    BankedMemory<0x8000, 0x2000, 0x0000> vram;

    std::vector<byte> oam;
    std::vector<byte> hram;

    Rom rom;
};
