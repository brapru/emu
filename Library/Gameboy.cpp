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
    , m_mmu(m_cartridge, m_cpu, m_ppu, m_timer, m_serial, m_joypad)
    , m_cpu(m_mmu, m_serial, m_timer, m_ppu)
    , m_serial(m_mmu)
    , m_timer(m_cpu)
    , m_ppu(m_cpu)
{
    if (!m_cartridge.is_loaded()) {
        outln("No rom data loaded");
    }

    m_interface = std::make_unique<Interface>(m_mmu, m_ppu);
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
    const auto set_input = [&](bool pressed, uint32_t key) {
        switch (key) {
        case SDLK_a:
        case SDLK_SPACE:
            m_joypad.set_button_state(ActionButtons::A, pressed);
            break;
        case SDLK_s:
        case SDLK_BACKSPACE:
            m_joypad.set_button_state(ActionButtons::B, pressed);
            break;
        case SDLK_RETURN:
            m_joypad.set_button_state(ActionButtons::Start, pressed);
            break;
        case SDLK_ESCAPE:
            m_joypad.set_button_state(ActionButtons::Select, pressed);
            break;
        case SDLK_UP:
            m_joypad.set_button_state(DirectionButtons::Up, pressed);
            break;
        case SDLK_DOWN:
            m_joypad.set_button_state(DirectionButtons::Down, pressed);
            break;
        case SDLK_LEFT:
            m_joypad.set_button_state(DirectionButtons::Left, pressed);
            break;
        case SDLK_RIGHT:
            m_joypad.set_button_state(DirectionButtons::Right, pressed);
            break;
        }
    };

    while (m_is_running) {

        usleep(1000);

        while (m_elapsed_cycles < CYCLES_PER_FRAME) {
            m_elapsed_cycles += m_cpu.cycle();
        };

        m_elapsed_cycles -= CYCLES_PER_FRAME;

        while (SDL_PollEvent(&m_event) > 0) {
            if (m_event.type == SDL_QUIT)
                m_is_running = false;
            if (m_event.type == SDL_KEYDOWN)
                set_input(true, m_event.key.keysym.sym);
            if (m_event.type == SDL_KEYUP)
                set_input(false, m_event.key.keysym.sym);
        }

        if (m_tracked_frame != m_ppu.current_frame()) {
            m_tracked_frame = m_ppu.current_frame();
            m_interface->update();
        }
    }
}