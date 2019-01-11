/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#pragma once

#include <SDL.h>
#include "sprite.hpp"
#include "brick.hpp"

#define UTIL_CLAMP(min, x, max) (((min) <= (x)) ? (((max) >= (x)) ? (x) : (max)) : (min))

#define CONST_WINDOW_TITLE "Metris"
#define CONST_WINDOW_WIDTH 700
#define CONST_WINDOW_HEIGHT 700

#define CONST_TXT_ATLAS "./atlas.bmp"
#define CONST_SFX_TURN "./turn.wav"
#define CONST_SFX_PLACE "./place.wav"
#define CONST_SFX_GAME_OVER "./go.wav"
#define CONST_SFX_MOVE "./move.wav"
#define CONST_SFX_BREAK "./place_break.wav"
#define CONST_SFX_BREAK4 "./place_4_break.wav"

#define CONST_MOD_MENU "./menu-theme.xm"
#define CONST_MOD_HS "./high-score.xm"
#define CONST_MOD_A_THEME "./a-theme.xm"

#define CONST_FIELD_WIDTH 10
#define CONST_FIELD_HEIGHT 18
#define CONST_BRICK_DIM 15

#define CONST_FIELD_W ((CONST_FIELD_WIDTH + 2) * CONST_BRICK_DIM) /* +2 Bricks for the walls */
#define CONST_FIELD_H (CONST_FIELD_HEIGHT * CONST_BRICK_DIM)

#define CONST_DIGIT_WIDTH 7
#define CONST_SIDE_BAR_W 6
#define CONST_DEFAULT_SIDE_BAR_WIDTH (CONST_BRICK_DIM * CONST_SIDE_BAR_W)

#define CONST_BRICKS 7

#define CONST_MAX_LEVEL 10
#define CONST_SEGMENT_W 7
#define CONST_SEGMENT_H 13

extern sprite_id const_j_brick_0[4][4];
extern sprite_id const_j_brick_90[4][4];
extern sprite_id const_j_brick_180[4][4];
extern sprite_id const_j_brick_270[4][4];
extern sprite_id const_l_brick_0[4][4];
extern sprite_id const_l_brick_90[4][4];
extern sprite_id const_l_brick_180[4][4];
extern sprite_id const_l_brick_270[4][4];
extern sprite_id const_t_brick_90[4][4];
extern sprite_id const_t_brick_0[4][4];
extern sprite_id const_t_brick_270[4][4];
extern sprite_id const_t_brick_180[4][4];
extern sprite_id const_s_brick_0[4][4];
extern sprite_id const_s_brick_90[4][4];
extern sprite_id const_z_brick_90[4][4];
extern sprite_id const_z_brick_0[4][4];
extern sprite_id const_square_brick_0[4][4];
extern sprite_id const_i_brick_90[4][4];
extern sprite_id const_i_brick_0[4][4];

sprite_id get_shape_for_type(sprite_id type, rotation rotation, int x, int y);

SDL_Point util_shift(SDL_Point p, int16_t x, int16_t y, int scale);
