#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

Gameboy::Gameboy(std::vector<uint8_t> rom_data)
    : m_cartridge(rom_data)
    , m_mmu(m_cartridge)
    , m_cpu(m_mmu)
{
    if (!m_cartridge.is_loaded()) {
        outln("Failed to load rom data");
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