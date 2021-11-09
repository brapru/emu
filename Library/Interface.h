#pragma once

#include <MMU.h>

#include <SDL2/SDL.h>

#include <memory>

constexpr uint16_t TILE_BASE_ADDRESS = 0x8000;

constexpr int SCREEN_WIDTH = 160;
constexpr int SCREEN_HEIGHT = 144;
constexpr int SCREEN_SCALE = 2;

constexpr unsigned long TILE_COLORS[4] = { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000 };

struct SDLWindowDeleter {
    inline void operator()(SDL_Window* window)
    {
        SDL_DestroyWindow(window);
    }
};

struct SDLRendererDeleter {
    inline void operator()(SDL_Renderer* renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
};

struct SDLTextureDeleter {
    inline void operator()(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
    }
};

class Interface {
public:
    Interface(MMU& mmu);

    void initialize();
    void event_handler();

    void update();

private:
    MMU& m_mmu;

    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_sdl_window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> m_sdl_renderer;
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> m_sdl_texture;

    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_sdl_tile_window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> m_sdl_tile_renderer;
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> m_sdl_tile_texture;
    std::unique_ptr<SDL_Surface> m_sdl_tile_surface;

    void update_sdl_tile_window();
    void render_tile(uint16_t tile_number, int x, int y);
};