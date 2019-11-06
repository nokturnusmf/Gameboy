#pragma once

#include <vector>

#include "gb.h"

template<unsigned Address, unsigned Block, unsigned Offset>
class BankedMemory {
public:
    BankedMemory(int size) : data(size) {}
    BankedMemory(std::vector<byte>&& data) : data(std::move(data)) {}

    byte& operator[](word address);

    void set_bank(int bank);

private:
    std::vector<byte> data;
    int bank = 0;
};

#include "bank.impl"
