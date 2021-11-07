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
    std::vector<uint8_t> empty;
    auto vm = std::make_unique<Gameboy>(empty);
    if (!vm) {
        outln("Failed to initialize the Gameboy.");
        exit(1);
    }

    vm->run();

    return 0;
}