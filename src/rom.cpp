#include "rom.h"

#include <iostream>
#include <iomanip>
#include <fstream>

static std::vector<unsigned char> load_file(const std::string& file_path) {
    std::ifstream file(file_path);

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> result(size);
    file.read(reinterpret_cast<char*>(&result[0]), size);

    return result;
}

Rom::Rom(const std::string& file_path) : data(load_file(file_path)) {
    std::cout << &data[0x134] << ' ' << std::hex << (int)type() << ' ' << (int)size() << ' ' << (int)ram_size() << '\n';
}

byte& Rom::operator[](word address) {
    return data[address < 0x4000 ? address : address + 0x4000 * bank];
}

byte Rom::read(word address) {
    if (address < 0x8000) {
        return (*this)[address];
    } else {
        return 0; // TODO
    }
}

void Rom::write(word address, byte value) {
    // TODO
}
