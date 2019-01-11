/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#include "star.hpp"
#include "sdl_helper.hpp"
#include <cstdio>

star::star()
{
    m_pos = {};
    m_init_tick = m_anim_time = 0;
    m_max_dim = {};
}

star::star(uint16_t x, uint16_t y, const float dim, const uint16_t ms)
{
    m_pos = {x, y};
    m_init_tick = SDL_GetTicks();
    m_anim_time = ms;
    m_max_dim = dim;
}

void star::draw(sdl_helper* helper) const
{
    if (m_init_tick == 0)
        return;

    const auto delta_t = SDL_GetTicks() - m_init_tick;

    auto percent = ((static_cast<float>(delta_t % m_anim_time) * 2.f) / m_anim_time);

    if (percent >= 1.f)
        percent = 2.f - percent;

    int dim = (m_max_dim * percent * helper->scale());

    if (dim <= 0)
        dim = 1;
    SDL_Rect temp = {
        helper->origin()->x + (m_pos.x * helper->scale()) - dim,
        helper->origin()->y + (m_pos.y * helper->scale()) - dim, dim * 2, dim * 2
    };

    SDL_RenderFillRect(helper->renderer(), &temp);
}
