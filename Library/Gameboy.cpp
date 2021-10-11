#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

Gameboy::Gameboy(std::string rom_file)
    : m_cartridge(rom_file)
    , m_mmu(m_cartridge)
    , m_cpu(m_mmu)
{
    if (!m_cartridge.is_loaded()) {
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
    m_cpu.cycle();
}