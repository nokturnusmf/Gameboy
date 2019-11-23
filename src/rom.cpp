#include "rom.h"

#include <fstream>

extern void error(const char* msg, int v);

static std::vector<byte> load_file(const std::string& file_path) {
    std::ifstream file(file_path);

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<byte> result(size);
    file.read(reinterpret_cast<char*>(&result[0]), size);

    return result;
}

class MBC {
public:
    virtual ~MBC() {}

    virtual byte read(word address) = 0;
    virtual void write(word address, byte value) = 0;
};

class NoMBC : public MBC {
public:
    NoMBC(std::vector<byte>&& data) : data(std::move(data)) {}

    byte read(word address) {
        return data[address];
    }

    void write(word, byte) {}

private:
    std::vector<byte> data;
};

class MBC3RB : public MBC {
public:
    MBC3RB(std::vector<byte>&& data, int ram_size)
        : data(std::move(data)), ram(ram_size == 3 ? 0x8000 : 0x800 * ram_size * ram_size) {

    }

    ~MBC3RB() {}

    byte read(word address) {
        return address < 0x8000 ? data[address] : ram[address];
    }

    void write(word address, byte value) {
        if (address >= 0xA000) {
            ram[address] = value;
        } else if (address < 0x2000) {
            // enable/disable ram
        } else if (address < 0x4000) {
            data.set_bank(value ? value : 1);
        } else if (address < 0x6000) {
            ram.set_bank(value);
        }
    }

private:
    BankedMemory<0x0000, 0x4000, 0x4000> data;
    BankedMemory<0xA000, 0x2000, 0x0000> ram;
};

MBC* create_mbc(std::vector<byte>&& data) {
    switch (data[0x147]) {
    case 0x00:
        return new NoMBC(std::move(data));

    case 0x13:
        return new MBC3RB(std::move(data), data[0x149]);

    default:
        error("unimplemented mbc type", data[0x147]);
        return 0;
    }
}

Rom::Rom(const std::string& file_path) {
    auto data = load_file(file_path);
    this->mbc = create_mbc(std::move(data));
}

Rom::~Rom() {
    delete mbc;
}

byte Rom::read(word address) {
    return mbc->read(address);
}

void Rom::write(word address, byte value) {
    mbc->write(address, value);
}
