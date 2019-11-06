#pragma once

#include <vector>
#include <string>

#include "gb.h"
#include "bank.h"

class Rom {
public:
    Rom(const std::string& file_path);

    byte read(word address);
    void write(word address, byte value);

    byte type() { return data[0x147]; }
    byte size() { return data[0x148]; }
    byte ram_size() { return data[0x149]; }

private:
    BankedMemory<0x0000, 0x4000, 0x4000> data;
};
