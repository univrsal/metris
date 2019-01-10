#pragma once

#include <SDL.h>

/* state() returns true
 * in given intervals
 */
class timer
{
public:
    timer();
    timer(int hold, int release, bool trigger_once);

    void set_hold(uint16_t h);
    void set_release(uint16_t r);
    bool state();
    void reset();

private:
    bool m_trigger_once;
    bool m_was_triggered = false;
    uint16_t m_hold, m_release;
    long m_init_ticks;
};
