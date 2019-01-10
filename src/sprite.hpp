#pragma once

#include <SDL.h>

enum sprite_id
{
    ID_EMPTY,
    ID_SQUARE_BRICK,
    ID_I_BRICK_TOP,
    ID_I_BRICK_MID,
    ID_I_BRICK_BTM,
    ID_I_BRICK_TOP_90,
    ID_I_BRICK_MID_90,
    ID_I_BRICK_BTM_90,
    ID_J_BRICK,
    ID_L_BRICK,
    ID_T_BRICK,
    ID_S_BRICK,
    ID_Z_BRICK,
    ID_WALL,
    ID_NUM_EMPTY,
    ID_NUM_0,
    ID_NUM_1,
    ID_NUM_2,
    ID_NUM_3,
    ID_NUM_4,
    ID_NUM_5,
    ID_NUM_6,
    ID_NUM_7,
    ID_NUM_8,
    ID_NUM_9,
    ID_START,
    ID_SCORE,
    ID_LEVEL,
    ID_LINES,
    ID_CORNER_TL,
    ID_SIDE_TOP,
    ID_CORNER_TR,
    ID_CORNER_BL,
    ID_SIDE_BOTTOM,
    ID_CORNER_BR,
    ID_SIDE_RIGHT,
    ID_SIDE_LEFT,
    ID_FILL,
    ID_GAME_OVER,
    ID_TRY_AGAIN,
    /*---*/
    ID_SPRITE_COUNT,
};

class atlas;

class sprite
{
public:
    sprite();
    sprite(uint16_t u, uint16_t v, uint16_t w, uint16_t h);
    SDL_Rect u_v{};
    void draw(uint16_t x, uint16_t y, atlas* a);
    void draw(const SDL_Point* p, atlas* a);
    void draw_absolute(const SDL_Point* p, atlas* a);
};
