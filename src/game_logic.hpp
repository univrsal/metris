#pragma once
#include "brick.hpp"
#include "animation.hpp"
#include "util.hpp"
#include "sprite.hpp"
#include "star.hpp"

class segment_display;
class bass_handler;
class atlas;
class sdl_helper;
class atlas;
class level;

enum game_state;
enum direction;

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
    void draw_rectangle(int x, int y, int w, int h); /* Draws a bordered rectangle */
    bool drop_brick();
    void do_game_over();
    bool in_game() const;
    bool game_over() const;
    void next_brick();
    void move_brick(direction d);
    void fast_drop();
    void place_brick();

    sprite_id m_game_field[CONST_FIELD_WIDTH][CONST_FIELD_HEIGHT];
    sprite m_sprites[ID_SPRITE_COUNT];

    brick m_current_brick;
    brick m_next_brick;
    animation m_gravity_timer;
    animation m_arrow;
    game_state m_state;
    
    level* m_current_level;
    bass_handler* m_bass;
    sdl_helper* m_helper;
    atlas* m_atlas;
    segment_display* m_score;
    segment_display* m_level;
    segment_display* m_lines;

    int m_game_over_y;
    
    long m_last_tick;
    long m_line_clear_start{};
    bool m_run_flag = true;
    bool m_game_over_state = false;
    bool m_lock_fast_drop;


    star m_stars[12];
};
