#pragma once

#include "SDL.h"

class sdl_helper;

class atlas
{
public:
    explicit atlas(sdl_helper* h);
    ~atlas();
    SDL_Texture* texture() const;

    bool load(const char* path);
    void draw(uint16_t x, uint16_t y, SDL_Rect* cut_out = nullptr) const;
    void draw_absolute(uint16_t x, uint16_t y, SDL_Rect* cut_out = nullptr) const;

private:
    SDL_Texture* m_sdl_texture = nullptr;
    sdl_helper* m_helper = nullptr;
};
