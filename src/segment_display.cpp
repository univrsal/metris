#include "sdl_helper.hpp"
#include "segment_display.hpp"
#include "game_logic.hpp"

void segment::draw(game_logic* logic, SDL_Point* p) const
{
    logic->get_sprite(static_cast<sprite_id>(m_digit +
        static_cast<int>(ID_NUM_0)))->draw_absolute(p, logic->get_atlas());
}

void segment::set_digit(const uint8_t d)
{
    m_digit = d;
}

uint8_t segment::get_digit() const
{
    return m_digit;
}

segment_display::segment_display(game_logic* logic, const int digits, const SDL_Point pos)
    : m_number(0)
{
    m_logic = logic;
    m_helper = logic->helper();
    m_pos = pos;
    for (auto i = 0; i < digits; i++)
        m_segments.emplace_back(new segment());
}

segment_display::~segment_display()
{
    m_segments.clear();
}

void segment_display::draw()
{
    auto origin = m_helper->field_origin();
    origin.x += m_helper->field_dim()->w + m_pos.x * m_helper->scale();
    origin.y += m_pos.y * m_helper->scale();
    auto leading_zero = true;

    for (auto& segment : m_segments)
    {
        if (segment->get_digit() > 0)
            leading_zero = false;
        
        origin.x += (CONST_DIGIT_WIDTH + 1) * m_helper->scale();
        if (segment->get_digit() == 0)
        {
            if ((leading_zero || m_display_leading_zeros))
                segment->draw(m_logic, &origin);
        }
        else
        {
            segment->draw(m_logic, &origin);
        }
    }
}

void segment_display::set_num(uint16_t num)
{
    m_number = num;
    int index = m_segments.size() - 1;
    do {
        const auto digit = num % 10;
        m_segments[index]->set_digit(digit);
        index--;
    } while ((num /= 10) && index < m_segments.size());
}

segment_display& segment_display::operator++()
{
    auto copy = m_number;
    set_num(++copy);
    return *this;
}
