#pragma once

#include <vector>
#include <string>

#include "gb.h"

class Rom {
public:
    Rom(const std::string& file_path);

    byte& operator[](word address);

    byte read(word address);
    void write(word address, byte value);

    byte type() { return data[0x147]; }
    byte size() { return data[0x148]; }
    byte ram_size() { return data[0x149]; }

private:
    std::vector<byte> data;

    int bank = 0;
};
