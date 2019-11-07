#include "rom.h"

#include <iostream>
#include <iomanip>
#include <fstream>

static std::vector<byte> load_file(const std::string& file_path) {
    std::ifstream file(file_path);

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<byte> result(size);
    file.read(reinterpret_cast<char*>(&result[0]), size);

    return result;
}

Rom::Rom(const std::string& file_path) : data(load_file(file_path)) {
    std::cout << &data[0x134] << ' ' << std::hex << (int)type() << ' ' << (int)size() << ' ' << (int)ram_size() << '\n';
}

byte Rom::read(word address) {
    return data[address];
}

void Rom::write(word address, byte value) {
    std::cout << "rom write " << address << ' ' << (int)value << '\n';
}
