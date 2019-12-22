#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <rom> <patch> (output)\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Couldn't read rom: " << argv[1] << '\n';
        return 2;
    }

    input.seekg(0, std::ios::end);
    auto size = input.tellg();
    input.seekg(0, std::ios::beg);

    std::vector<unsigned char> rom(size);
    input.read(reinterpret_cast<char*>(&rom[0]), size);
    input.close();

    std::ifstream patch(argv[2]);
    if (!patch) {
        std::cerr << "Couldn't read patch: " << argv[2] << '\n';
        return 3;
    }

    std::string line;
    while (!patch.eof()) {
        std::getline(patch, line);
        if (line.size() && line[0] != '-') {
            char* end;
            auto address = strtol(line.data(), &end, 16);
            unsigned char val = strtol(end, 0, 16);

            rom[address] = val;
        }
    }

    std::ofstream output(argc > 3 ? argv[3] : argv[1]);
    output.write(reinterpret_cast<const char*>(&rom[0]), size);
}
