#include <Interface.h>
#include <MMU.h>
#include <Utils/Format.h>

Interface::Interface(MMU& mmu)
    : m_mmu(mmu)
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
            (SCREEN_WIDTH * 4),
            (SCREEN_HEIGHT * 4),
            SDL_WINDOW_SHOWN));

    if (m_sdl_tile_window == nullptr) {
        outln("Failed to create SDL window");
        exit(1);
    }

    m_sdl_tile_surface = std::unique_ptr<SDL_Surface>(
        SDL_CreateRGBSurface(
            0,
            SCREEN_WIDTH * 4,
            SCREEN_HEIGHT * 4,
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
            SCREEN_WIDTH,
            SCREEN_HEIGHT));

    if (m_sdl_texture == nullptr) {
        outln("Failed to create SDL Texture");
        exit(1);
    }

    m_sdl_tile_texture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>(
        SDL_CreateTexture(
            m_sdl_tile_renderer.get(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH * SCREEN_SCALE,
            SCREEN_HEIGHT * SCREEN_SCALE));

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

void Interface::draw_tile(SDL_Surface* surface, uint16_t start, uint16_t tile_number, int x, int y)
{
    SDL_Rect rectangle;

    for (int y_tile = 0; y_tile < 16; y_tile += 2) {
        uint8_t byte1 = m_mmu.read(start + (tile_number * 16) + y_tile);
        uint8_t byte2 = m_mmu.read(start + (tile_number * 16) + y_tile + 1);

        for (int bit = 7; bit >= 0; bit--) {
            uint8_t hi = !!(byte1 & (1 << bit)) << 1;
            uint8_t lo = !!(byte2 & (1 << bit)) << 1;

            uint8_t color = hi | lo;

            rectangle.x = x + ((7 - bit) * SCREEN_SCALE);
            rectangle.y = y + (y_tile / 2 * SCREEN_SCALE);
            rectangle.w = SCREEN_SCALE;
            rectangle.h = SCREEN_SCALE;

            SDL_FillRect(surface, &rectangle, TILE_COLORS[color]);
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

    uint16_t address;

    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 16; x++) {
            draw_tile(m_sdl_tile_surface.get(), address, tile_number, x_pos + (x * SCREEN_SCALE), y_pos + (y * SCREEN_SCALE));
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

void Interface::update()
{
    update_sdl_tile_window();
}