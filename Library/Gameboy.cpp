#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

Gameboy::Gameboy(std::string rom_file)
{
    Cartridge::the().load_rom_file(rom_file);
    if (!Cartridge::the().is_loaded()) {
        outln("Failed to load rom file: {}", rom_file);
        exit(1);
    }
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