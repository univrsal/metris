#pragma once
#include <cstdint>

class game_logic;

class level
{
public:
    level(game_logic* logic);
    void up();
    uint8_t id() const;
    uint16_t drop_speed() const;

    uint8_t check_line_clears();
    void draw_line_clear();
    void clear_lines();
    uint32_t get_score() const;
    uint16_t get_lines() const;

private:
    uint16_t score_for_line_clear(uint8_t lines) const;
    uint8_t m_id;
    uint32_t m_min_score;
    uint16_t m_tile_drop_speed;
    uint32_t m_score = 0;
    uint16_t m_lines = 0;
    game_logic* m_logic;

    uint8_t m_cleared_lines[4];
    uint8_t m_cleared_line_count;
};
