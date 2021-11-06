#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

constexpr int SCREEN_WIDTH = 160;
constexpr int SCREEN_HEIGHT = 144;
constexpr int SCREEN_SCALE = 2;

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
    void initialize();
    void event_handler();

private:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_sdl_window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> m_sdl_renderer;
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> m_sdl_texture;
};