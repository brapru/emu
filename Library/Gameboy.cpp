#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

std::unique_ptr<Gameboy> Gameboy::initialize(std::string rom_file)
{
    auto gameboy = std::make_unique<Gameboy>();

    Cartridge::the().load_rom_file(rom_file);
    if (!Cartridge::the().is_loaded()) {
        outln("Failed to load rom file: {}", rom_file);
        return nullptr;
    }

    return gameboy;
}

void Gameboy::run(void)
{
    while (true) {
        main_cycle();
    }
}

void Gameboy::main_cycle(void)
{
    CPU::the().cycle();
}