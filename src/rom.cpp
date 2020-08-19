#include "rom.h"

#include <fstream>
#include <chrono>

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

class MBCBase : public MBC {
public:
    MBCBase(std::vector<byte>&& data, int ram_size, const std::string& save_path)
        : rom(std::move(data)), ram(ram_size == 3 ? 0x8000 : 0x800 * ram_size * ram_size), save_path(save_path) {
        std::ifstream file(save_path);
        if (file) {
            file.read(reinterpret_cast<char*>(&ram[0xA000]), ram.size());
        }
    }

    virtual ~MBCBase() {
        std::ofstream file(save_path);
        if (file) {
            ram.set_bank(0);
            file.write(reinterpret_cast<const char*>(&ram[0xA000]), ram.size());
        }
    }

protected:
    BankedMemory<0x0000, 0x4000, 0x4000> rom;
    BankedMemory<0xA000, 0x2000, 0x0000> ram;

private:
    std::string save_path;
};

class MBC1 : public MBCBase {
public:
    MBC1(std::vector<byte>&& data, int ram_size, const std::string& save_path)
        : MBCBase(std::move(data), ram_size, save_path) {}

    byte read(word address) {
        return address < 0x8000 ? rom[address] : ram[address];
    }

    void write(word address, byte value) {
        if (address >= 0xA000) {
            ram[address] = value;
        } else if (address < 0x2000) {
            // enable/disable ram
        } else if (address < 0x4000) {
            lower = value & 0x1F;
            this->set_banks();
        } else if (address < 0x6000) {
            upper = value & 0x03;
            this->set_banks();
        } else if (address < 0x8000) {
            mode = value;
            this->set_banks();
        }
    }

private:
    void set_banks() {
        if (mode) {
            rom.set_bank(lower);
            ram.set_bank(upper);
        } else {
            ram.set_bank(0);
            rom.set_bank(lower | (upper << 5));
        }
    }

    bool mode = false;
    byte lower = 0;
    byte upper = 0;
};

class MBC3 : public MBCBase {
public:
    MBC3(std::vector<byte>&& data, int ram_size, const std::string& save_path)
        : MBCBase(std::move(data), ram_size, save_path) {}

    byte read(word address) {
        if (address < 0x8000) {
            return rom[address];
        } else if (rtc) {
            return current_time(rtc);
        } else {
            return ram[address];
        }
    }

    void write(word address, byte value) {
        if (address >= 0xA000) {
            if (!rtc) {
                ram[address] = value;
            }
        } else if (address < 0x2000) {
            // enable/disable ram
        } else if (address < 0x4000) {
            rom.set_bank(value ? value : 1);
        } else if (address < 0x6000) {
            if (value <= 3) {
                ram.set_bank(value);
                rtc = 0;
            } else {
                rtc = value;
            }
        }
    }

private:
    static byte current_time(int rtc) {
        using namespace std::chrono;

        auto now = system_clock::now();
        auto tt = system_clock::to_time_t(now);
        auto local = localtime(&tt);

        switch (rtc) {
        case 0x08:
            return local->tm_sec;
        case 0x09:
            return local->tm_min;
        case 0x0A:
            return local->tm_hour;
        case 0x0B:
            return local->tm_yday;
        case 0x0C:
            return local->tm_yday & 0x100;
        default:
            return 0;
        }
    }

    int rtc = 0;
};

class MBC5 : public MBCBase {
public:
    MBC5(std::vector<byte>&& data, int ram_size, const std::string& save_path)
        : MBCBase(std::move(data), ram_size, save_path) {}

    byte read(word address) {
        return address < 0x8000 ? rom[address] : ram[address];
    }

    void write(word address, byte value) {
        if (address >= 0xA000) {
            ram[address] = value;
        } else if (address < 0x2000) {
            // enable/disable ram
        } else if (address < 0x3000) {
            lower = value;
            rom.set_bank((upper << 8) | lower);
        } else if (address < 0x4000) {
            upper = value;
            rom.set_bank((upper << 8) | lower);
        } else if (address < 0x6000) {
            ram.set_bank(value);
        }
    }

private:
    byte lower = 0;
    byte upper = 0;
};

std::unique_ptr<MBC> create_mbc(const std::string& file_path) {
    auto data = load_file(file_path);
    switch (data[0x147]) {
    case 0x00:
        return std::make_unique<NoMBC>(std::move(data));

    case 0x01:
    case 0x02:
    case 0x03:
    case 0x08:
    case 0x09:
        return std::make_unique<MBC1>(std::move(data), data[0x149], file_path + ".sav");

    case 0x0F:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
        return std::make_unique<MBC3>(std::move(data), data[0x149], file_path + ".sav");

    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
        return std::make_unique<MBC5>(std::move(data), data[0x149], file_path + ".sav");

    default:
        error("unimplemented mbc type", data[0x147]);
        return nullptr;
    }
}

Rom::Rom(const std::string& file_path) : mbc(create_mbc(file_path)) {}
Rom::~Rom() = default;

byte Rom::read(word address) {
    return mbc->read(address);
}

void Rom::write(word address, byte value) {
    mbc->write(address, value);
}
