#pragma once

#include <SDL.h>

class animation
{
public:
    animation();

    animation(const int hold, const int release, const bool trigger_once);

    void set_hold(const uint16_t h);

    void set_release(const uint16_t r);

    bool state(void);

    void reset(void);

private:
    bool m_trigger_once;
    bool m_was_triggered = false;
    uint16_t m_hold, m_release;
    long m_init_ticks;
};