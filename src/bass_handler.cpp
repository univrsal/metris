#include <cstdio>
#include <string>
#include "bass_handler.hpp"
#include "util.hpp"

bass_handler::bass_handler()
{
    m_current = MUSIC_NONE;
}

bool bass_handler::init()
{
    bool success = false;
#ifndef _WIN32
    int a = 1;
    BASS_DEVICEINFO info; 

    while (BASS_GetDeviceInfo(a, &info)) /* find first device, that can be initialized */
    {
       if (info.flags & BASS_DEVICE_ENABLED)
       {
           printf("[BASS] Found audio device %i::%s\n", a, info.name);
           if (strstr(info.name, "Pulse") != nullptr)
           {
               printf("[BASS] Initializing %i::\"%s\"...\n", a, info.name);
               success = BASS_Init(a, 44100, 0, nullptr, nullptr);
               break;
           }
       }
       a++;
    }
#else
    printf("[BASS] Initializing default audio device\n");
    success = BASS_Init(1, 44100, BASS_DEVICE_STEREO, nullptr, nullptr);
#endif
    printf("[BASS] Audio device state: %s\n", success ? "Initialized" : "Initialization failed");

    if (success)
    {
        m_menu_theme = BASS_MusicLoad(FALSE, CONST_MOD_MENU, 0, 0, BASS_SAMPLE_LOOP, 1);
        m_high_score = BASS_MusicLoad(FALSE, CONST_MOD_HS, 0, 0, BASS_SAMPLE_LOOP, 1);
        m_theme_a = BASS_MusicLoad(FALSE, CONST_MOD_A_THEME, 0, 0, BASS_SAMPLE_LOOP, 1);

        m_move_sample = BASS_StreamCreateFile(FALSE, CONST_SFX_MOVE, 0, 0, BASS_SAMPLE_MONO);
        m_game_over_sample = BASS_StreamCreateFile(FALSE, CONST_SFX_GAME_OVER, 0, 0, BASS_SAMPLE_MONO);
        m_place_sample = BASS_StreamCreateFile(FALSE, CONST_SFX_PLACE, 0, 0, BASS_SAMPLE_MONO);
        m_turn_sample = BASS_StreamCreateFile(FALSE, CONST_SFX_TURN, 0, 0, BASS_SAMPLE_MONO);
        m_break_sample = BASS_StreamCreateFile(FALSE, CONST_SFX_BREAK, 0, 0, BASS_SAMPLE_MONO);
        m_break4_sample = BASS_StreamCreateFile(FALSE, CONST_SFX_BREAK4, 0, 0, BASS_SAMPLE_MONO);

        success = m_menu_theme != NULL && m_high_score != NULL && m_theme_a != NULL && m_move_sample != NULL
            && m_game_over_sample != NULL && m_place_sample != NULL && m_turn_sample != NULL;
    }

    if (!success)
    {
        printf("BASS initialization failed! Error Code: %i\n", BASS_ErrorGetCode());
    }
    else
    {
        switch_music(MUSIC_MENU);
    }
    return success;
}

void bass_handler::close_bass() const
{
    pause();
    BASS_MusicFree(m_high_score);
    BASS_MusicFree(m_menu_theme);
    BASS_MusicFree(m_theme_a);

    BASS_SampleFree(m_game_over_sample);
    BASS_SampleFree(m_move_sample);
    BASS_SampleFree(m_place_sample);
    BASS_SampleFree(m_break_sample);
    BASS_SampleFree(m_break4_sample);

    BASS_Free();
}

void bass_handler::play(const music_type m)
{
    m_current = m;
    m_playing = true;
    switch (m)
    {
    case MUSIC_MENU:
        if (!BASS_ChannelPlay(m_menu_theme, TRUE))
            printf("Error playing menu theme! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case MUSIC_A_THEME:
        if (!BASS_ChannelPlay(m_theme_a, TRUE))
            printf("Error playing theme a! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case MUSIC_HIGH_SCORE:
        if (!BASS_ChannelPlay(m_high_score, TRUE))
            printf("Error playing high score theme! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    default: ;
    }
}

void bass_handler::play_sfx(const sfx_type sfx) const
{
    switch (sfx)
    {
    case SFX_MOVE:
        if (!BASS_ChannelPlay(m_move_sample, TRUE))
            printf("Error playing move sample! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case SFX_PLACE:
        if (!BASS_ChannelPlay(m_place_sample, TRUE))
            printf("Error playing place sample! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case SFX_GAME_OVER:
        if (!BASS_ChannelPlay(m_game_over_sample, TRUE))
            printf("Error playing game over sample! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case SFX_TURN:
        if (!BASS_ChannelPlay(m_turn_sample, TRUE))
            printf("Error playing turn sample! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case SFX_BREAK:
        if (!BASS_ChannelPlay(m_break_sample, TRUE))
            printf("Error playing break sample! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    case SFX_BREAK4:
        if (!BASS_ChannelPlay(m_break4_sample, TRUE))
            printf("Error playing break4 sample! Error Code: %i\n", BASS_ErrorGetCode());
        break;
    }
}

void bass_handler::pause() const
{
    BASS_ChannelPause(m_menu_theme);
    BASS_ChannelPause(m_theme_a);
    BASS_ChannelPause(m_high_score);
}

void bass_handler::set_vol(const float f) const
{
    BASS_ChannelSetAttribute(m_theme_a, BASS_ATTRIB_VOL, f);
    BASS_ChannelSetAttribute(m_high_score, BASS_ATTRIB_VOL, f);
    BASS_ChannelSetAttribute(m_menu_theme, BASS_ATTRIB_VOL, f);
}

void bass_handler::toggle_music(void)
{
    m_playing = !m_playing;

    if (m_playing)
        play(m_current);
    else
        pause();
}

void bass_handler::switch_music(const music_type m)
{
    pause();

    m_current = m;
    switch (m_current)
    {
    case MUSIC_MENU:
        set_vol(.3f);
        break;
    case MUSIC_A_THEME:
        set_vol(.1f);
        break;
    case MUSIC_HIGH_SCORE:
        set_vol(.4f);
        break;
    default: ;
    }
    play(m_current);
}
