#pragma once
#include <SDL.h>

class sdl_helper;

class star
{
public:
    star();
    star(uint16_t x, uint16_t y, float dim, uint16_t ms);
    void draw(sdl_helper* helper) const;

private:
    SDL_Point m_pos{};
    uint32_t m_init_tick;
    float m_max_dim;
    uint16_t m_anim_time;
};
