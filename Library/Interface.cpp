#include <Interface.h>
#include <MMU.h>
#include <PPU.h>
#include <Utils/Format.h>

Interface::Interface(MMU& mmu, PPU& ppu)
    : m_mmu(mmu)
    , m_ppu(ppu)
{
}

void Interface::initialize()
{
    SDL_Init(SDL_INIT_VIDEO);

    m_sdl_window = std::unique_ptr<SDL_Window, SDLWindowDeleter>(
        SDL_CreateWindow(
            "emu",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH * SCREEN_SCALE,
            SCREEN_HEIGHT * SCREEN_SCALE,
            SDL_WINDOW_SHOWN));

    if (m_sdl_window == nullptr) {
        outln("Failed to create SDL window");
        exit(1);
    }

    int x_pos, y_pos;
    SDL_GetWindowPosition(m_sdl_window.get(), &x_pos, &y_pos);

    m_sdl_tile_window = std::unique_ptr<SDL_Window, SDLWindowDeleter>(
        SDL_CreateWindow(
            "emu tiles",
            x_pos + (SCREEN_WIDTH * SCREEN_SCALE),
            y_pos,
            TILE_SCREEN_WIDTH * TILE_SCREEN_SCALE,
            TILE_SCREEN_HEIGHT * TILE_SCREEN_SCALE,
            SDL_WINDOW_SHOWN));

    if (m_sdl_tile_window == nullptr) {
        outln("Failed to create SDL window");
        exit(1);
    }

    m_sdl_surface = std::unique_ptr<SDL_Surface>(
        SDL_CreateRGBSurface(
            0,
            SCREEN_WIDTH * SCREEN_SCALE,
            SCREEN_HEIGHT * SCREEN_SCALE,
            32,
            0x00FF0000,
            0x0000FF00,
            0x000000FF,
            0xFF000000));

    m_sdl_tile_surface = std::unique_ptr<SDL_Surface>(
        SDL_CreateRGBSurface(
            0,
            (TILE_SCREEN_WIDTH * TILE_SCREEN_SCALE) + (16 * TILE_SCREEN_SCALE),
            (TILE_SCREEN_HEIGHT * TILE_SCREEN_SCALE) - (32 * TILE_SCREEN_SCALE),
            32,
            0x00FF0000,
            0x0000FF00,
            0x000000FF,
            0xFF000000));

    m_sdl_renderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>(
        SDL_CreateRenderer(
            m_sdl_window.get(),
            -1,
            SDL_RENDERER_ACCELERATED));

    if (m_sdl_renderer == nullptr) {
        outln("Failed to create SDL Renderer");
        exit(1);
    }

    m_sdl_tile_renderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>(
        SDL_CreateRenderer(
            m_sdl_tile_window.get(),
            -1,
            SDL_RENDERER_ACCELERATED));

    if (m_sdl_tile_renderer == nullptr) {
        outln("Failed to create SDL Renderer");
        exit(1);
    }

    m_sdl_texture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>(
        SDL_CreateTexture(
            m_sdl_renderer.get(),
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH * SCREEN_SCALE,
            SCREEN_HEIGHT * SCREEN_SCALE));

    if (m_sdl_texture == nullptr) {
        outln("Failed to create SDL Texture");
        exit(1);
    }

    m_sdl_tile_texture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>(
        SDL_CreateTexture(
            m_sdl_tile_renderer.get(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            (TILE_SCREEN_WIDTH * TILE_SCREEN_SCALE) + (16 * TILE_SCREEN_SCALE),
            (TILE_SCREEN_HEIGHT * TILE_SCREEN_SCALE) - (32 * TILE_SCREEN_SCALE)));

    if (m_sdl_tile_texture == nullptr) {
        outln("Failed to create SDL Texture");
        exit(1);
    }
}

void Interface::event_handler()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT)
            exit(1);
    }
}

void Interface::render_tile(uint16_t tile_number, int x, int y)
{
    SDL_Rect rectangle;

    // Each tile occupies 16 bytes, where each line is represented by 2 bytes
    for (int y_tile = 0; y_tile < 16; y_tile += 2) {
        uint8_t byte1 = m_mmu.read(TILE_BASE_ADDRESS + (tile_number * 16) + y_tile);
        uint8_t byte2 = m_mmu.read(TILE_BASE_ADDRESS + (tile_number * 16) + y_tile + 1);

        for (int bit = 7; bit >= 0; bit--) {
            uint8_t hi = !!(byte1 & (1 << bit)) << 1;
            uint8_t lo = !!(byte2 & (1 << bit));

            uint8_t color = hi | lo;

            rectangle.x = x + ((7 - bit) * SCREEN_SCALE);
            rectangle.y = y + (y_tile / 2 * SCREEN_SCALE);
            rectangle.w = SCREEN_SCALE;
            rectangle.h = SCREEN_SCALE;

            SDL_FillRect(m_sdl_tile_surface.get(), &rectangle, TILE_COLORS[color]);
        }
    }
}

void Interface::update_sdl_tile_window()
{
    int x_pos = 0, y_pos = 0, tile_number = 0;

    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = m_sdl_tile_surface->w;
    rectangle.h = m_sdl_tile_surface->h;
    SDL_FillRect(m_sdl_tile_surface.get(), &rectangle, 0xFF111111);

    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 16; x++) {
            render_tile(tile_number, x_pos + (x * SCREEN_SCALE), y_pos + (y * SCREEN_SCALE));
            x_pos += (8 * SCREEN_SCALE);
            tile_number++;
        }
        y_pos += (8 * SCREEN_SCALE);
        x_pos = 0;
    }

    SDL_UpdateTexture(m_sdl_tile_texture.get(), NULL, m_sdl_tile_surface->pixels, m_sdl_tile_surface->pitch);
    SDL_RenderClear(m_sdl_tile_renderer.get());
    SDL_RenderCopy(m_sdl_tile_renderer.get(), m_sdl_tile_texture.get(), NULL, NULL);
    SDL_RenderPresent(m_sdl_tile_renderer.get());
}

void Interface::update_sdl_window()
{
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = SCREEN_SCALE;
    rectangle.h = SCREEN_SCALE;

    uint32_t* display_buffer = m_ppu.display_buffer();

    for (int i = 0; i < LCD::DISPLAY_Y_RESOLUTION; i++) {
        for (int j = 0; j < LCD::DISPLAY_X_RESOLUTION; j++) {
            rectangle.x = j * SCREEN_SCALE;
            rectangle.y = i * SCREEN_SCALE;
            rectangle.w = SCREEN_SCALE;
            rectangle.h = SCREEN_SCALE;

            SDL_FillRect(m_sdl_surface.get(), &rectangle, display_buffer[j + (i * LCD::DISPLAY_X_RESOLUTION)]);
        }
    }

    SDL_UpdateTexture(m_sdl_texture.get(), NULL, m_sdl_surface->pixels, m_sdl_surface->pitch);
    SDL_RenderClear(m_sdl_renderer.get());
    SDL_RenderCopy(m_sdl_renderer.get(), m_sdl_texture.get(), NULL, NULL);
    SDL_RenderPresent(m_sdl_renderer.get());
}

void Interface::update()
{
    update_sdl_window();
    update_sdl_tile_window();
}