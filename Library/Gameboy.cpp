#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

#ifdef __EMSCRIPTEN__
#    include <emscripten.h>
#endif

Gameboy::Gameboy(std::vector<uint8_t> rom_data)
    : m_cartridge(rom_data)
    , m_mmu(m_cartridge, m_cpu, m_ppu, m_timer, m_serial)
    , m_cpu(m_mmu, m_serial)
    , m_serial(m_mmu)
    , m_timer(m_cpu)
{
    if (!m_cartridge.is_loaded()) {
        outln("No rom data loaded");
    }

    m_interface = std::make_unique<Interface>(m_mmu);
    if (m_interface == nullptr) {
        outln("Failed to inititalize the interface");
        exit(1);
    }
}

void main_cycle_wasm(void* arg)
{
    if (!static_cast<Gameboy*>(arg)->has_cartridge())
        return;

    static_cast<Gameboy*>(arg)->main_cycle();
}

void Gameboy::run(void)
{
    m_interface->initialize();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&main_cycle_wasm, this, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    while (true) {
        main_cycle();
    }
#endif
}

void Gameboy::main_cycle(void)
{
    m_cpu.cycle();

    m_interface->event_handler();
    m_interface->update();
}