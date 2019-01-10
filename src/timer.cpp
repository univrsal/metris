#include "timer.hpp"

timer::timer()
{
    m_hold = 0;
    m_release = 0;
    m_init_ticks = 0;
    m_trigger_once = false;
}

timer::timer(const int hold, const int release, const bool trigger_once)
{
    m_hold = hold;
    m_release = release;
    m_init_ticks = SDL_GetTicks();
    m_trigger_once = trigger_once;
}

void timer::set_hold(const uint16_t h)
{
    m_hold = h;
}

void timer::set_release(const uint16_t r)
{
    m_release = r;
}

bool timer::state()
{
    if (SDL_GetTicks() - m_init_ticks < m_hold)
    {
        if (m_trigger_once && !m_was_triggered)
        {
            m_was_triggered = true;
            return true;
        }
        if (!m_trigger_once)
        {
            return true;
        }
    }

    if (SDL_GetTicks() - m_init_ticks - m_hold < m_release)
        return false;

    if (SDL_GetTicks() - m_init_ticks - m_hold > m_release && SDL_GetTicks() - m_init_ticks > m_hold)
    {
        reset();
        m_was_triggered = false;
        return false;
    }
    return false;
}

void timer::reset()
{
    m_init_ticks = SDL_GetTicks();
}
