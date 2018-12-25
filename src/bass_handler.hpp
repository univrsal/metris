#pragma once

#include <bass.h>

enum music_type
{
    MUSIC_NONE,
    MUSIC_MENU,
    MUSIC_A_THEME,
    MUSIC_HIGH_SCORE
};

enum sfx_type
{
    SFX_MOVE,
    SFX_PLACE,
    SFX_GAME_OVER,
    SFX_TURN,
    SFX_BREAK,
    SFX_BREAK4
};

class bass_handler
{
public:
    bass_handler();

    bool init();
    void close_bass() const;

    void play(music_type m);
    void play_sfx(sfx_type sfx) const;
    void pause(music_type m) const;
    void set_vol(float f) const;
    void toggle_music();
    void switch_music(music_type m);

private:
    bool m_playing = true;
    music_type m_current;

    HMUSIC m_menu_theme{};
    HMUSIC m_theme_a{};
    HMUSIC m_high_score{};

    HSTREAM m_move_sample{};
    HSTREAM m_place_sample{};
    HSTREAM m_game_over_sample{};
    HSTREAM m_turn_sample{};
    HSTREAM m_break_sample{};
    HSTREAM m_break4_sample{};

};
