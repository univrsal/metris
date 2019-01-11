/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#pragma once
#include <SDL.h>
#include <vector>
#include <memory>

class game_logic;
class sdl_helper;

class segment
{
public:
    segment() = default;
    void draw(game_logic* logic, SDL_Point* p) const;
    void set_digit(uint8_t d);
    uint8_t get_digit() const;
private:
    uint8_t m_digit = 0;
};

/* 7 segment display for numbers */
class segment_display
{
public:
    segment_display(game_logic* logic, int digits, SDL_Point pos);
    ~segment_display();

    void draw();
    void set_num(uint16_t num);

    segment_display& operator++();
private:
    bool m_display_leading_zeros = true;
    sdl_helper* m_helper;
    game_logic* m_logic;
    SDL_Point m_pos{};
    uint16_t m_number;
    std::vector<std::unique_ptr<segment>> m_segments;
};
