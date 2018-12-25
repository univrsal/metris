#include <cstdio>
#include <cmath>
#include "atlas.hpp"
#include "sdl_helper.hpp"

atlas::atlas(sdl_helper* h)
{
    m_sdl_texture = nullptr;
    m_helper = h;
}

atlas::~atlas()
{
    SDL_DestroyTexture(m_sdl_texture);
}

SDL_Texture* atlas::texture(void) const
{
    return m_sdl_texture;
}

bool atlas::load(const char* path)
{
    auto success = true;
    auto* surface = SDL_LoadBMP(path);

    if (!surface)
    {
        printf("Couldn't load atlas %s, SDL Error: %s\n", path, SDL_GetError());
        success = false;
    }
    else
    {
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

        m_sdl_texture = SDL_CreateTextureFromSurface(m_helper->renderer(), surface);

        if (!m_sdl_texture)
        {
            printf("Couldn't load atlas %s, SDL Error: %s\n", path, SDL_GetError());
            success = false;
        }
        else
        {
            SDL_FreeSurface(surface);
        }
    }

    return success;
}

void atlas::draw(const uint16_t x, const uint16_t y, SDL_Rect* cut_out) const
{
    if (cut_out != nullptr)
    {
        auto dst = m_helper->scale_rect(cut_out);
        dst.x = x;
        dst.y = y;
        SDL_RenderCopy(m_helper->renderer(), m_sdl_texture, cut_out, &dst);
    }
    else
    {
        SDL_RenderCopy(m_helper->renderer(), m_sdl_texture, cut_out, nullptr);
    }
}

void atlas::draw_rot_nocenter(const uint16_t x, const uint16_t y, const int angle, SDL_Rect* cut_out) const
{
    if (cut_out != nullptr)
    {
        SDL_Point center = {0, 0};
        auto dst = m_helper->scale_rect(cut_out);
        dst.x = x;
        dst.y = y;
        SDL_RenderCopyEx(m_helper->renderer(), m_sdl_texture, cut_out, &dst,
            static_cast<double>(angle), &center, SDL_FLIP_NONE);
    }
}

void atlas::draw_rot(const uint16_t x, const uint16_t y, const int angle, SDL_Rect* cut_out) const
{
    if (cut_out != nullptr)
    {
        const auto x_center = static_cast<int>(std::round((cut_out->w * m_helper->scale()) / 2.0));
        const auto y_center = static_cast<int>(std::round(((cut_out->h * m_helper->scale()) / 2.0)));

        SDL_Point center = {x_center, y_center};
        auto dst = m_helper->scale_rect(cut_out);
        dst.x = x;
        dst.y = y;
        SDL_RenderCopyEx(m_helper->renderer(), m_sdl_texture, cut_out, &dst,
            static_cast<double>(angle), &center, SDL_FLIP_NONE);
    }
}

void atlas::draw_absolute(const uint16_t x, const uint16_t y, SDL_Rect* cut_out) const
{
    if (cut_out != nullptr)
    {
        auto dst = m_helper->scale_rect(cut_out);
        dst.x = x;
        dst.y = y;
        SDL_RenderCopy(m_helper->renderer(), m_sdl_texture, cut_out, &dst);
    }
    else
    {
        SDL_RenderCopy(m_helper->renderer(), m_sdl_texture, cut_out, nullptr);
    }
}
