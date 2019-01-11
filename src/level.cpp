/* This file is part of Metris
 * licensed under the GPL 2.0
 * github.com/univrsal/metris
 */

#include "level.hpp"
#include "game_logic.hpp"
#include "sdl_helper.hpp"

level::level(game_logic* logic)
{
    m_id = 0;
    m_min_score = 0;
    m_tile_drop_speed = 1000;
    m_logic = logic;
}

void level::up()
{
    m_id++;
    m_min_score = 1000 * m_id;
    if (m_tile_drop_speed > 100)
        m_tile_drop_speed -= 100;
}

uint8_t level::id() const
{
    return m_id;
}

uint16_t level::get_lines() const
{
    return m_lines;
}

void level::reset()
{
    m_score = 0;
    m_lines = 0;
    m_min_score = 0;
    m_id = 0;
    m_tile_drop_speed = 1000;
    m_cleared_line_count = 0;
}

uint16_t level::score_for_line_clear(uint8_t lines) const
{
    switch (lines)
    {
    case 1:
        return (m_id + 1) * 40;
    case 2:
        return (m_id + 1) * 100;
    case 3:
        return (m_id + 1) * 300;
    case 4:
        return (m_id + 1) * 1200;
    default:
        return 0;
    }
}

uint16_t level::drop_speed() const
{
    return m_tile_drop_speed;
}

uint8_t level::check_line_clears()
{
    m_cleared_line_count = 0;
    for (auto y = 0; y < CONST_FIELD_HEIGHT; y++)
    {
        auto filled_tiles = 0;
        for (auto x = 0; x < CONST_FIELD_WIDTH; x++)
        {
            if (m_logic->get_at(x, y) != ID_EMPTY)
            {
                filled_tiles++;
            }
        }

        if (filled_tiles >= CONST_FIELD_WIDTH)
        {
            m_cleared_lines[m_cleared_line_count++] = y;
        }
    }

    return m_cleared_line_count;
}

void level::draw_line_clear()
{
    auto* bright = m_logic->helper()->color_bright();

    for (auto i = 0; i < m_cleared_line_count; i++)
    {
        auto line = *m_logic->helper()->field_dim();
        line.h = CONST_BRICK_DIM * m_logic->helper()->scale();
        line.y += m_cleared_lines[i] * CONST_BRICK_DIM * m_logic->helper()->scale();

        SDL_SetRenderDrawColor(m_logic->helper()->renderer(), bright->r, bright->g, bright->b, bright->a);
        SDL_RenderFillRect(m_logic->helper()->renderer(), &line);
    }
}

void level::clear_lines()
{
    for (auto line = 0; line < m_cleared_line_count; line++) /* Clear filled lines */
    {
        for (auto x = 0; x < CONST_FIELD_WIDTH; x++)
        {
            m_logic->set_at(x, m_cleared_lines[line], ID_EMPTY);
        }
    }

    for (auto y = CONST_FIELD_HEIGHT - 2; y >= 0; y--) /* Bottom row can be skipped*/
    {
        auto line_empty = 0;
        for (auto y2 = y + 1; y2 < CONST_FIELD_HEIGHT; y2++) /* Check all lines below and stop at first non empty */
        {
            auto flag = true;
            for (auto x = 0; x < CONST_FIELD_WIDTH; x++)
            {
                if (m_logic->get_at(x, y2) != ID_EMPTY)
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
                line_empty = y2;
        }

        if (line_empty > 0) /* Move current line down */
        {
            for (auto x = 0; x < CONST_FIELD_WIDTH; x++)
            {
                m_logic->set_at(x, line_empty, m_logic->get_at(x, y));
                m_logic->set_at(x, y, ID_EMPTY);
            }
        }
    }

    m_score += score_for_line_clear(m_cleared_line_count);
    m_lines += m_cleared_line_count;

    if (m_score >= m_min_score)
    {
        up();
    }
    m_cleared_line_count = 0;
}

uint32_t level::get_score() const
{
    return m_score;
}
