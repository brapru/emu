#include <Gameboy.h>
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

    auto vm = std::make_unique<Gameboy>(rom_file);
    if (!vm) {
        outln("Failed to initialize the Gameboy.");
        exit(1);
    }

    vm->run();

    return 0;
}