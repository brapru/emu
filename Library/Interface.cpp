#include <Interface.h>
#include <Utils/Format.h>

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