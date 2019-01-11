/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#pragma once
#include <SDL.h>
#include "sprite.hpp"

class game_logic;

enum rotation
{
    ROT_0 = 0,
    ROT_90 = 90,
    ROT_180 = 180,
    ROT_270 = 270
};

enum direction
{
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

class sdl_helper;

class brick
{
public:
    brick();
    brick(sprite_id type, uint16_t x, uint16_t y);

    void draw(sdl_helper* helper, game_logic* logic) const;
    void rotate(game_logic* logic);
    void shuffle();
    void set_type(sprite_id s);
    sprite_id get_type() const;
    rotation get_rotation() const;
    void set_grid_pos(int x, int y, game_logic* logic);
    void resize(game_logic* logic);

    int16_t grid_x() const;
    int16_t grid_y() const;
private:
    SDL_Point m_pos;
    sprite_id m_type;
    rotation m_rotation;
    int16_t m_grid_x, m_grid_y;
};
