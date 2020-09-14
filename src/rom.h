#pragma once

#include <vector>
#include <string>
#include <memory>

#include "types.h"
#include "bank.h"

class MBC;

class Rom {
public:
    Rom(const std::string& file_path);
    Rom(const Rom& other);
    ~Rom();

    Rom& operator=(const Rom& other);

    byte read(word address);
    void write(word address, byte value);

private:
    std::unique_ptr<MBC> mbc;
};
