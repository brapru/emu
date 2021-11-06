#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

Gameboy::Gameboy(std::vector<uint8_t> rom_data)
    : m_cartridge(rom_data)
    , m_mmu(m_cartridge, m_cpu, m_timer, m_serial)
    , m_cpu(m_mmu, m_serial)
    , m_serial(m_mmu)
    , m_timer(m_cpu)
{
    if (!m_cartridge.is_loaded()) {
        outln("Failed to load rom data");
        exit(1);
    }

    m_interface = std::make_unique<Interface>();
    if (m_interface == nullptr) {
        outln("Failed to inititalize the interface");
        exit(1);
    }
}

void Gameboy::run(void)
{
    m_interface->initialize();

    while (true) {
        main_cycle();
    }
}

void Gameboy::main_cycle(void)
{
    m_cpu.cycle();

    m_interface->event_handler();
}