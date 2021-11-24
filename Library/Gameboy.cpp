#include <CPU.h>
#include <Cartridge.h>
#include <Gameboy.h>
#include <Utils/Format.h>

#include <thread>
#include <unistd.h>

#ifdef __EMSCRIPTEN__
#    include <emscripten.h>
#endif

Gameboy::Gameboy(std::vector<uint8_t> rom_data)
    : m_cartridge(rom_data)
    , m_mmu(m_cartridge, m_cpu, m_ppu, m_timer, m_serial)
    , m_cpu(m_mmu, m_serial, m_timer, m_ppu)
    , m_serial(m_mmu)
    , m_timer(m_cpu)
    , m_ppu(m_cpu)
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

    main_cycle();
}

void Gameboy::main_cycle(void)
{
    while (m_is_running) {

        usleep(1000);

        while (m_elapsed_cycles < CYCLES_PER_FRAME) {
            m_elapsed_cycles += m_cpu.cycle();
        };

        m_elapsed_cycles -= CYCLES_PER_FRAME;

        while (SDL_PollEvent(&m_event) > 0) {
            if (m_event.type == SDL_QUIT)
                m_is_running = false;
        }

        if (m_tracked_frame != m_ppu.current_frame()) {
            m_tracked_frame = m_ppu.current_frame();
            m_interface->update();
        }
    }
}