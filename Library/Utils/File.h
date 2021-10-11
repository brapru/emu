#include <fstream>
#include <string>

#include <Utils/Format.h>

inline std::vector<uint8_t> read_rom_into_vector(std::string const& filename)
{
    std::ifstream rom_file(filename, std::ios::in | std::ios::binary);

    if (!rom_file) {
        outln("Could not load rom file: {}", filename);
        exit(1);
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(rom_file)), std::istreambuf_iterator<char>());
    outln("Loaded {} bytes from ROM file: {}", data.size(), filename);

    return data;
}