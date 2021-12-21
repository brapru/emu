#include <fstream>
#include <string>

#include <Utils/Format.h>

inline std::vector<uint8_t> read_rom_into_vector(std::string const& filename)
{
    std::ifstream rom_file(filename, std::ios::in | std::ios::binary);

    if (rom_file.fail())
        return {};

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(rom_file)), std::istreambuf_iterator<char>());
    outln("Loaded {} bytes from ROM file: {}", data.size(), filename);

    return data;
}

inline void write_vector_into_file(std::vector<uint8_t> data, std::string const& filename)
{
    std::ofstream rom_file(filename, std::ios::out | std::ios::binary);
    std::ostream_iterator<uint8_t> output_iterator(rom_file);
    std::copy(data.begin(), data.end(), output_iterator);
}