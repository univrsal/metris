#pragma once
#include "brick.hpp"
#include "timer.hpp"
#include "util.hpp"
#include "sprite.hpp"
#include "star.hpp"

class segment_display;
class bass_handler;
class atlas;
class sdl_helper;
class atlas;
class level;

enum game_state
{
    STATE_IN_MENU,
    STATE_INGAME,
    STATE_GAMEOVER,
    STATE_GAME_ENDING,
    STATE_LINE_BREAK
};

class game_logic
{
public:
    game_logic(sdl_helper* helper);
    ~game_logic();
    bool init();

    void run();
    void handle_events();
    void draw();

    level* lvl() const;

    brick* current();

    brick* next();

    bass_handler* bass() const;
    sdl_helper* helper() const;

    sprite_id get_at(int x, int y);
    void set_at(int x, int y, sprite_id id);
    sprite* get_sprite(sprite_id id);
    atlas* get_atlas() const;
    bool run_flag() const;
private:
    void start();
    void draw_start();
    void draw_walls();
    void draw_sidebar();
    void draw_gameover();
    void draw_rectangle(int x, int y, int w, int h); /* Draws a bordered rectangle (Used in sidebar) */
    bool drop_brick();
    void do_game_over(); /* Fills the game field */
    bool in_game() const;
    bool game_over() const;
    void next_brick();
    void move_brick(direction d);
    void fast_drop(); /* Drop brick faster, if player presses down 's' or 'arrow down' */
    void place_brick();

    sprite_id m_game_field[CONST_FIELD_WIDTH][CONST_FIELD_HEIGHT];
    sprite m_sprites[ID_SPRITE_COUNT];

    brick m_current_brick;
    brick m_next_brick;
    timer m_gravity_timer;
    timer m_arrow;
    game_state m_state;

    level* m_current_level;
    bass_handler* m_bass;
    sdl_helper* m_helper;
    atlas* m_atlas;

    segment_display* m_score;
    segment_display* m_level;
    segment_display* m_lines;

    int m_game_over_y;

    long m_line_clear_start{};
    long m_game_over_start{}; /* used to check when to start game over music */
    bool m_run_flag = true;
    bool m_lock_fast_drop;

    star m_stars[12];
};
