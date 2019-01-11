/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#include "sdl_helper.hpp"
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "util.hpp"

bool sdl_helper::init(const char* title, int w, int h)
{
    auto success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Initialization of SDL failed! Error: %s\n", SDL_GetError());
        success = false;
    }

    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr)
    {
        printf("Creating SDL Window failed! Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        SDL_SetWindowMinimumSize(m_window, CONST_DEFAULT_SIDE_BAR_WIDTH + CONST_FIELD_W, CONST_FIELD_H);
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }

    if (m_renderer == nullptr)
    {
        printf("Creating SDL Renderer failed! Error: %s\n", SDL_GetError());
        success = false;
    }

    m_window_surface = SDL_GetWindowSurface(m_window);

    if (m_window_surface == nullptr)
    {
        printf("Couldn't get window surface! Error: %s\n", SDL_GetError());
        success = false;
    }

    srand(time(nullptr));
    m_window_id = SDL_GetWindowID(m_window);
    return success;
}

void sdl_helper::close() const
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

SDL_Window* sdl_helper::window() const
{
    return m_window;
}

SDL_Renderer* sdl_helper::renderer() const
{
    return m_renderer;
}

SDL_Surface* sdl_helper::surface() const
{
    return m_window_surface;
}

SDL_Event* sdl_helper::event()
{
    return &m_event;
}

void sdl_helper::handle_events()
{
    if (m_event.type == SDL_WINDOWEVENT)
    {
        if (m_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
            && m_event.window.windowID == m_window_id)
        {
            m_window_dim.w = m_event.window.data1;
            m_window_dim.h = m_event.window.data2;
            set_display(m_display_dim.w, m_display_dim.h);
        }
    }
}

uint8_t sdl_helper::scale() const
{
    return m_scale;
}

void sdl_helper::clear() const
{
    SDL_SetRenderDrawColor(m_renderer, m_dark.r, m_dark.g, m_dark.b, m_dark.a);
    SDL_RenderClear(m_renderer);
}

void sdl_helper::repaint() const
{
    SDL_RenderPresent(m_renderer);
}

SDL_Rect sdl_helper::scale_rect(const SDL_Rect* r) const
{
    return {r->x * m_scale, r->y * m_scale, r->w * m_scale, r->h * m_scale};
}

const SDL_Color* sdl_helper::color_bright() const
{
    return &m_bright;
}

const SDL_Color* sdl_helper::color_gray() const
{
    return &m_gray;
}

const SDL_Color* sdl_helper::color_mid() const
{
    return &m_mid;
}

const SDL_Color* sdl_helper::color_dark() const
{
    return &m_dark;
}

const SDL_Color* sdl_helper::color_darkest() const
{
    return &m_darkest;
}

const SDL_Point* sdl_helper::origin() const
{
    return &m_origin;
}

SDL_Point sdl_helper::field_origin() const
{
    return SDL_Point{m_field_dim.x, m_field_dim.y};
}

const SDL_Rect* sdl_helper::game_dim() const
{
    return &m_game_dim;
}

const SDL_Rect* sdl_helper::field_dim() const
{
    return &m_field_dim;
}

void sdl_helper::set_display(const int w, const int h)
{
    m_display_dim.w = w;
    m_display_dim.h = h;

    auto scale_w = m_window_dim.w / m_display_dim.w;
    auto scale_h = m_window_dim.h / m_display_dim.h;

    if (scale_w > 0 && scale_h > 0)
        m_scale = SDL_min(scale_w, scale_h);
    m_origin = {
        m_window_dim.w / 2 - (m_display_dim.w * m_scale) / 2, m_window_dim.h / 2 - (m_display_dim.h * m_scale) / 2
    };

    const auto new_w = CONST_FIELD_W * m_scale;
    const auto new_h = CONST_FIELD_H * m_scale;
    const auto new2_w = new_w + CONST_DEFAULT_SIDE_BAR_WIDTH * m_scale;

    m_field_dim.x = m_origin.x + CONST_BRICK_DIM * m_scale;
    m_field_dim.y = m_window_dim.h / 2 - new_h / 2;
    m_field_dim.w = CONST_FIELD_WIDTH * CONST_BRICK_DIM * m_scale;
    m_field_dim.h = new_h;

    m_game_dim = {m_origin.x, m_field_dim.y, new2_w, new_h};
}
