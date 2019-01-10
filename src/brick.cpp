#include "util.hpp"
#include "brick.hpp"
#include "sdl_helper.hpp"
#include "game_logic.hpp"
#include <cstdlib>

brick::brick()
{
    m_type = ID_EMPTY;
    m_pos = {0, 0};
    m_rotation = ROT_0;
}

brick::brick(sprite_id type, uint16_t x, uint16_t y)
{
    m_type = type;
    m_pos = {x, y};
    m_rotation = ROT_0;
}

void brick::draw(sdl_helper* helper, game_logic* logic) const
{
    const auto p = helper->field_origin();
    SDL_Rect r = {p.x, p.y, CONST_BRICK_DIM * helper->scale(), CONST_BRICK_DIM * helper->scale()};

    for (auto x = 0; x < 4; x++)
    {
        for (auto y = 0; y < 4; y++)
        {
            const auto current = get_shape_for_type(m_type, m_rotation, x, y);
            r.x = p.x + (x + m_grid_x) * CONST_BRICK_DIM * helper->scale();
            r.y = p.y + (y + m_grid_y) * CONST_BRICK_DIM * helper->scale();

            if (current != ID_EMPTY && y + m_grid_y >= 0)
            {
                auto shifted = util_shift(p, (x + m_grid_x) * CONST_BRICK_DIM,
                    (y + m_grid_y) * CONST_BRICK_DIM, helper->scale());
                logic->get_sprite(current)->draw_absolute(&shifted, logic->get_atlas());
            }
        }
    }
}

void brick::rotate(game_logic* logic)
{
    const auto new_rot = m_rotation == ROT_0
        ? ROT_90
        : (m_rotation == ROT_90 ? ROT_180 : (m_rotation == ROT_180 ? ROT_270 : ROT_0));

    for (auto x = 0; x < 4; x++)
    {
        for (auto y = 0; y < 4; y++)
        {
            if (m_grid_y + y < 0)
                continue;

            const auto brick_part = get_shape_for_type(m_type, new_rot, x, y);
            const auto field_part = logic->get_at(m_grid_x + x, m_grid_y + y);

            if (m_grid_x + x > CONST_FIELD_WIDTH - 1 && brick_part != ID_EMPTY)
                return;

            if (m_grid_x + x < 0 && brick_part != ID_EMPTY)
                return;

            if (m_grid_y + y > CONST_FIELD_HEIGHT - 1 && brick_part != ID_EMPTY)
                return;

            if (field_part != ID_EMPTY && brick_part != ID_EMPTY)
                return;
        }
    }
    m_rotation = new_rot;
}

void brick::shuffle()
{
    const auto i = 0 + (rand() % static_cast<int>(6 + 1));
    m_rotation = ROT_0;
    switch (i)
    {
    case 0:
        m_type = ID_I_BRICK_BTM;
        break;
    case 1:
        m_type = ID_L_BRICK;
        break;
    case 2:
        m_type = ID_J_BRICK;
        break;
    case 3:
        m_type = ID_S_BRICK;
        break;
    case 4:
        m_type = ID_Z_BRICK;
        break;
    case 5:
        m_type = ID_SQUARE_BRICK;
        break;
    case 6:
        m_type = ID_T_BRICK;
        break;
    default: ;
    }
}

void brick::set_type(const sprite_id s)
{
    m_type = s;
}

sprite_id brick::get_type() const
{
    return m_type;
}

rotation brick::get_rotation() const
{
    return m_rotation;
}

void brick::set_grid_pos(const int x, const int y, game_logic* logic)
{
    m_grid_x = x;
    m_grid_y = y;

    m_pos.x = logic->helper()->field_origin().x + CONST_BRICK_DIM * m_grid_x * logic->helper()->scale();
    m_pos.y = logic->helper()->field_origin().x + CONST_BRICK_DIM * m_grid_y * logic->helper()->scale();
}

void brick::resize(game_logic* logic)
{
    set_grid_pos(m_grid_x, m_grid_y, logic);
}

int16_t brick::grid_x() const
{
    return m_grid_x;
}

int16_t brick::grid_y() const
{
    return m_grid_y;
}
