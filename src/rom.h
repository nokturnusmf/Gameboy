#pragma once

#include <vector>
#include <string>

#include "types.h"
#include "bank.h"

class MBC;

class Rom {
public:
    Rom(const std::string& file_path);
    ~Rom();

    byte read(word address);
    void write(word address, byte value);

private:
    MBC* mbc;
};
