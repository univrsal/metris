/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#pragma once
#include <SDL.h>

class sdl_helper /* Helper class for SDL methods */
{
public:
    sdl_helper() = default;
    ~sdl_helper();

    bool init(const char* title, int w, int h);
    void close() const;

    SDL_Window* window() const;
    SDL_Renderer* renderer() const;
    SDL_Surface* surface() const;
    SDL_Event* event();
    void handle_events();
    uint8_t scale() const;

    void clear() const;
    void repaint() const;

    SDL_Rect scale_rect(const SDL_Rect* r) const;

    const SDL_Color* color_bright() const;
    const SDL_Color* color_gray() const;
    const SDL_Color* color_mid() const;
    const SDL_Color* color_dark() const;
    const SDL_Color* color_darkest() const;

    const SDL_Point* origin() const;
    SDL_Point field_origin() const;
    const SDL_Rect* game_dim() const;
    const SDL_Rect* field_dim() const;

    void set_display(int w, int h);
private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Surface* m_window_surface = nullptr;
    SDL_Event m_event{};

    SDL_Point m_origin{};
    SDL_Rect m_display_dim = {0, 0, 153, 138}; /* The total unscaled area of the current screen*/
    SDL_Rect m_field_dim{};
    SDL_Rect m_game_dim{}; /* Scaled area and position of the screen */
    SDL_Rect m_window_dim{};

    SDL_Color m_bright = {133, 158, 181, 255};
    SDL_Color m_gray = {80, 112, 143, 255};
    SDL_Color m_mid = {42, 77, 110};
    SDL_Color m_dark = {16, 48, 78};
    SDL_Color m_darkest = {3, 15, 46};

    uint8_t m_scale = 1;
    uint32_t m_window_id = 0;
};

inline sdl_helper::~sdl_helper()
{
    close();
}
