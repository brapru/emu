#include <Gameboy.h>
#include <Utils/File.h>
#include <Utils/Format.h>

#include <iostream>
#include <memory>
#include <string>

void usage(void)
{
    outln("./gbc++ ROM_FILE");
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage();
        exit(1);
    }

    std::string rom_file = argv[1];

    auto rom_data = read_rom_into_vector(rom_file);
    if (rom_data.empty()) {
        outln("Failed to load the rom data from file: {}", rom_file);
        exit(1);
    }

    auto vm = std::make_unique<Gameboy>(rom_data);

    if (!vm || !vm->has_cartridge()) {
        outln("Failed to initialize the Gameboy.");
        exit(1);
    }

    vm->run();

    return 0;
}