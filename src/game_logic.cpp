#include <SDL.h>
#include "level.hpp"
#include "atlas.hpp"
#include "game_logic.hpp"
#include "bass_handler.hpp"
#include "sprite.hpp"
#include "sdl_helper.hpp"
#include "segment_display.hpp"

game_logic::game_logic(sdl_helper* helper)
    : m_game_field{}, m_lock_fast_drop(false)
{
    m_bass = new bass_handler();
    m_atlas = new atlas(helper);
    m_current_level = nullptr;
    m_arrow = timer(1000, 1000, false);
    m_helper = helper;
    m_state = STATE_IN_MENU;

    m_score = new segment_display(this, 6, SDL_Point{28, 30});
    m_level = new segment_display(this, 6, SDL_Point{28, 90});
    m_lines = new segment_display(this, 6, SDL_Point{28, 150});

    m_game_over_y = 0;
}

game_logic::~game_logic()
{
    delete m_bass;
    delete m_atlas;
    delete m_score;
    delete m_level;
    delete m_lines;
    delete m_current_level;
}

bool game_logic::init()
{
    if (m_bass->init())
    {
        if (m_atlas->load(CONST_TXT_ATLAS))
        {
            /* Initialize Sprite locations */
            m_sprites[ID_SQUARE_BRICK] = {CONST_BRICK_DIM * 0, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_I_BRICK_TOP] = {CONST_BRICK_DIM * 1, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_I_BRICK_MID] = {CONST_BRICK_DIM * 2, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_I_BRICK_BTM] = {CONST_BRICK_DIM * 3, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_J_BRICK] = {CONST_BRICK_DIM * 4, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_L_BRICK] = {CONST_BRICK_DIM * 5, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_T_BRICK] = {CONST_BRICK_DIM * 6, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_S_BRICK] = {CONST_BRICK_DIM * 7, 0, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_Z_BRICK] = {CONST_BRICK_DIM * 0, CONST_BRICK_DIM, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_I_BRICK_TOP_90] = {CONST_BRICK_DIM * 1, CONST_BRICK_DIM, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_I_BRICK_MID_90] = {CONST_BRICK_DIM * 2, CONST_BRICK_DIM, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_I_BRICK_BTM_90] = {CONST_BRICK_DIM * 3, CONST_BRICK_DIM, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_WALL] = {60, 15, CONST_BRICK_DIM, CONST_BRICK_DIM};
            m_sprites[ID_START] = {0, 30, 153, 138};
            m_sprites[ID_SCORE] = {154, 16, 38, 10};
            m_sprites[ID_LEVEL] = {154, 28, 38, 10};
            m_sprites[ID_LINES] = {154, 40, 38, 10};
            m_sprites[ID_GAME_OVER] = {154, 51, 32, 22};
            m_sprites[ID_TRY_AGAIN] = {154, 74, 65, 42};

            for (uint16_t i = ID_CORNER_TL; i <= ID_FILL; i++)
                m_sprites[i] = {
                    static_cast<uint16_t>(120 + CONST_BRICK_DIM * (i - ID_CORNER_TL)), 0, CONST_BRICK_DIM,
                    CONST_BRICK_DIM
                };

            for (uint16_t i = ID_NUM_EMPTY; i <= ID_NUM_9; i++)
                m_sprites[i] = {
                    static_cast<uint16_t>(75 + CONST_SEGMENT_W * (i - ID_NUM_EMPTY)), CONST_BRICK_DIM, CONST_SEGMENT_W,
                    CONST_SEGMENT_H
                };

            m_bass->play(MUSIC_MENU);
            m_current_level = new level(this);
            m_gravity_timer = timer(m_current_level->drop_speed() / 2, m_current_level->drop_speed() / 2, true);

            m_stars[0] = star(40 + (rand() % 10 - 5), 60 + (rand() % 10 - 5), 1.2, 2500);
            m_stars[1] = star(50 + (rand() % 10 - 5), 80 + (rand() % 10 - 5), 1, 3500);
            m_stars[2] = star(40 + (rand() % 10 - 5), 80 + (rand() % 10 - 5), .8, 3000);
            m_stars[3] = star(70 + (rand() % 10 - 5), 60 + (rand() % 10 - 5), 1.1, 4000);
            m_stars[4] = star(90 + (rand() % 10 - 5), 80 + (rand() % 10 - 5), .8, 2000);
            m_stars[5] = star(60 + (rand() % 10 - 5), 60 + (rand() % 10 - 5), 1.1, 3400);
            m_stars[6] = star(50 + (rand() % 10 - 5), 85 + (rand() % 10 - 5), 1.2, 2600);
            m_stars[7] = star(30 + (rand() % 10 - 5), 85 + (rand() % 10 - 5), 1.2, 2900);
            m_stars[8] = star(15 + (rand() % 10 - 5), 90 + (rand() % 10 - 5), 1.0, 2200);
            m_stars[9] = star(100 + (rand() % 10 - 5), 60 + (rand() % 10 - 5), 1.2, 3900);
            m_stars[10] = star(125 + (rand() % 10 - 5), 60 + (rand() % 10 - 5), 1.0, 2200);
            m_stars[11] = star(135 + (rand() % 10 - 5), 70 + (rand() % 10 - 5), 1.2, 2000);

            m_level->set_num(1);
            return true;
        }
    }
    return false;
}

void game_logic::run()
{
    switch (m_state)
    {
    case STATE_INGAME:
        if (m_gravity_timer.state())
        {
            if (!drop_brick())
            {
                place_brick();
                m_bass->play_sfx(SFX_PLACE);
                next_brick();
            }
        }

        break;
    case STATE_LINE_BREAK:
        if (SDL_GetTicks() - m_line_clear_start > 1300)
        {
            m_current_level->clear_lines();
            m_score->set_num(m_current_level->get_score());
            m_lines->set_num(m_current_level->get_lines());
            m_level->set_num(m_current_level->id());
            m_gravity_timer = timer(m_current_level->drop_speed() / 2, m_current_level->drop_speed() / 2, true);
            m_state = STATE_INGAME;
        }
        break;
    case STATE_GAME_ENDING:
        do_game_over();
        if (SDL_GetTicks() - m_game_over_start > 2700)
        {
            m_state = STATE_GAMEOVER;
            m_bass->switch_music(MUSIC_HIGH_SCORE);
        }
        break;
    default: ;
    }
}

void game_logic::start()
{
    m_state = STATE_INGAME;
    m_gravity_timer.reset();
    m_next_brick.set_grid_pos(CONST_FIELD_WIDTH + 2, CONST_FIELD_HEIGHT - 5, this);
    m_next_brick.shuffle();
    next_brick();
}

bool game_logic::in_game() const
{
    return m_state != STATE_IN_MENU;
}

bool game_logic::game_over() const
{
    return m_state == STATE_GAMEOVER || m_state == STATE_GAME_ENDING;
}

void game_logic::next_brick()
{
    m_current_brick.set_type(m_next_brick.get_type());
    m_next_brick.shuffle();
    m_gravity_timer.reset();

    switch (m_current_brick.get_type()) /* Position each type of brick correctly, so they're centered */
    {
    case ID_I_BRICK_BTM:
        m_current_brick.set_grid_pos(3, -1, this);
        break;
    case ID_J_BRICK:
        m_current_brick.set_grid_pos(4, 1, this);
        break;
    case ID_L_BRICK:
        m_current_brick.set_grid_pos(4, 1, this);
        break;
    case ID_SQUARE_BRICK:
        m_current_brick.set_grid_pos(4, 1, this);
        break;
    case ID_T_BRICK:
        m_current_brick.set_grid_pos(3, 1, this);
        break;
    case ID_S_BRICK:
        m_current_brick.set_grid_pos(4, 1, this);
        break;
    case ID_Z_BRICK:
        m_current_brick.set_grid_pos(4, 1, this);
        break;
    default: ;
    }
}

void game_logic::draw_start()
{
    m_sprites[ID_START].draw_absolute(m_helper->origin(), m_atlas);
    SDL_SetRenderDrawColor(m_helper->renderer(), m_helper->color_bright()->r, m_helper->color_bright()->g,
        m_helper->color_bright()->b, m_helper->color_bright()->a);
    if (m_arrow.state())
    {
        SDL_Rect temp = {
            m_helper->origin()->x + 45 * m_helper->scale(),
            m_helper->origin()->y + 106 * m_helper->scale(), 4 * m_helper->scale(), 6 * m_helper->scale()
        };

        SDL_RenderFillRect(m_helper->renderer(), &temp);
    }

    for (auto s : m_stars)
        s.draw(m_helper);
}

void game_logic::draw_walls()
{
    SDL_Point temp = {};

    for (auto i = 0; i < CONST_FIELD_HEIGHT; i++)
    {
        temp.x = m_helper->origin()->x;
        temp.y = m_helper->origin()->y + CONST_BRICK_DIM * i * m_helper->scale();
        m_sprites[ID_WALL].draw_absolute(&temp, m_atlas);
        temp.x += CONST_BRICK_DIM * (CONST_FIELD_WIDTH + 1) * m_helper->scale();
        m_sprites[ID_WALL].draw_absolute(&temp, m_atlas);
    }
}

void game_logic::draw_sidebar()
{
    auto o = m_helper->field_origin();
    for (auto y = 0; y < CONST_FIELD_HEIGHT; y++)
    {
        for (auto x = 1; x <= CONST_SIDE_BAR_W; x++) /* Start at one, because 0 is the brick wall */
        {
            auto temp = o;
            temp.x += m_helper->field_dim()->w + x * CONST_BRICK_DIM * m_helper->scale();
            temp.y += y * m_helper->scale() * CONST_BRICK_DIM;
            m_sprites[ID_FILL].draw_absolute(&temp, m_atlas);
        }
    }
    o.x += m_helper->field_dim()->w;

    draw_rectangle(1, 0, 5, 3);
    draw_rectangle(1, 4, 5, 3);
    draw_rectangle(1, 8, 5, 3);
    draw_rectangle(1, 12, 5, 5);

    auto temp = util_shift(o, 40, 17, m_helper->scale());
    m_sprites[ID_SCORE].draw_absolute(&temp, m_atlas);
    temp = util_shift(o, 40, 77, m_helper->scale());
    m_sprites[ID_LEVEL].draw_absolute(&temp, m_atlas);
    temp = util_shift(o, 40, 137, m_helper->scale());
    m_sprites[ID_LINES].draw_absolute(&temp, m_atlas);

    m_score->draw();
    m_level->draw();
    m_lines->draw();
}

void game_logic::draw_gameover()
{
    const auto o = m_helper->origin();
    m_sprites[ID_GAME_OVER].draw(o->x + 73 * m_helper->scale(),
        o->y + 60 * m_helper->scale(), m_atlas);
    m_sprites[ID_TRY_AGAIN].draw(o->x + 55 * m_helper->scale(),
        o->y + 120 * m_helper->scale(), m_atlas);
}

void game_logic::draw_rectangle(int x, int y, int w, int h)
{
    auto p = util_shift(m_helper->field_origin(), x * CONST_BRICK_DIM,
        y * CONST_BRICK_DIM, m_helper->scale());
    auto temp = p;
    p.x += m_helper->field_dim()->w;

    auto* b = m_helper->color_bright();
    SDL_SetRenderDrawColor(m_helper->renderer(), b->r, b->g, b->b, b->a);
    SDL_Rect rect = {
        p.x + CONST_BRICK_DIM * m_helper->scale(), p.y + CONST_BRICK_DIM * m_helper->scale(),
        w * CONST_BRICK_DIM * m_helper->scale(), h * CONST_BRICK_DIM * m_helper->scale()
    };
    SDL_RenderFillRect(m_helper->renderer(), &rect);

    for (auto x2 = 0; x2 < w; x2++)
    {
        temp = util_shift(p, x2 * CONST_BRICK_DIM, 0, m_helper->scale());
        m_sprites[ID_SIDE_TOP].draw_absolute(&temp, m_atlas);
        temp = util_shift(p, x2 * CONST_BRICK_DIM, h * CONST_BRICK_DIM, m_helper->scale());
        m_sprites[ID_SIDE_BOTTOM].draw_absolute(&temp, m_atlas);
    }

    for (auto y2 = 0; y2 < h; y2++)
    {
        temp = util_shift(p, 0, y2 * CONST_BRICK_DIM, m_helper->scale());
        m_sprites[ID_SIDE_RIGHT].draw_absolute(&temp, m_atlas);
        temp = util_shift(p, w * CONST_BRICK_DIM, y2 * CONST_BRICK_DIM, m_helper->scale());
        m_sprites[ID_SIDE_LEFT].draw_absolute(&temp, m_atlas);
    }

    /* Corners */
    m_sprites[ID_CORNER_TL].draw_absolute(&p, m_atlas);
    temp = util_shift(p, 0, h * CONST_BRICK_DIM, m_helper->scale());
    m_sprites[ID_CORNER_BL].draw_absolute(&temp, m_atlas);

    temp = util_shift(p, w * CONST_BRICK_DIM, 0, m_helper->scale());
    m_sprites[ID_CORNER_TR].draw_absolute(&temp, m_atlas);
    temp = util_shift(p, w * CONST_BRICK_DIM, h * CONST_BRICK_DIM, m_helper->scale());
    m_sprites[ID_CORNER_BR].draw_absolute(&temp, m_atlas);
}

bool game_logic::drop_brick(void)
{
    auto success = true;
    const auto new_y_grid = m_current_brick.grid_y() + 1;
    const int grid_x = m_current_brick.grid_x();

    for (auto x = 0; x < 4; x++)
    {
        for (auto y = 0; y < 4; y++)
        {
            const auto brick_part =
                get_shape_for_type(m_current_brick.get_type(), m_current_brick.get_rotation(), x, y);
            const auto field_part = m_game_field[grid_x + x][new_y_grid + y];

            if (brick_part != ID_EMPTY && field_part != ID_EMPTY || brick_part != ID_EMPTY && new_y_grid + y >=
                CONST_FIELD_HEIGHT)
            {
                success = false;
                m_lock_fast_drop = true; /* Prevent next brick from falling immediately */
                goto end; // I was told this is okay, and by that I mean stack overflow said so
            }
        }
    }

end:

    if (success)
    {
        m_current_brick.set_grid_pos(grid_x, new_y_grid, this);
    }

    return success;
}

void game_logic::move_brick(const direction d)
{
    if (d == DIR_RIGHT || d == DIR_LEFT)
    {
        const auto offset = d == DIR_RIGHT ? 1 : -1;
        const auto new_x_grid = m_current_brick.grid_x() + offset;
        const int grid_y = m_current_brick.grid_y();

        for (auto x = 0; x < 4; x++)
        {
            for (auto y = 0; y < 4; y++)
            {
                if (m_current_brick.grid_y() + y < 0)
                    continue; /* This part is outside the field -> no collision */

                const auto brick_part = get_shape_for_type(m_current_brick.get_type(), m_current_brick.get_rotation(),
                    x, y);
                const auto field_part = m_game_field[new_x_grid + x][grid_y + y];

                // What the fuck
                if (new_x_grid < 0)
                {
                    if (brick_part != ID_EMPTY && new_x_grid + x < 0)
                        return;
                }
                else if (new_x_grid + x >= CONST_FIELD_WIDTH)
                {
                    if (brick_part != ID_EMPTY)
                        return;
                }
                else if (brick_part != ID_EMPTY && field_part != ID_EMPTY)
                {
                    return;
                }
            }
        }
        m_current_brick.set_grid_pos(new_x_grid, grid_y, this);
        m_bass->play_sfx(SFX_MOVE);
    }
    else if (d == DIR_DOWN)
    {
        drop_brick();
    }
}

void game_logic::fast_drop()
{
    if (SDL_GetTicks() % 100 < 90)
    {
        if (!drop_brick())
        {
            place_brick();
            m_bass->play_sfx(SFX_PLACE);
            next_brick();
        }
    }
}

void game_logic::place_brick()
{
    const int grid_y = m_current_brick.grid_y();
    const int grid_x = m_current_brick.grid_x();

    for (auto x = 0; x < 4; x++)
    {
        for (auto y = 0; y < 4; y++)
        {
            const auto brick_part =
                get_shape_for_type(m_current_brick.get_type(), m_current_brick.get_rotation(), x, y);
            const auto field_part = m_game_field[grid_x + x][grid_y + y];

            if (brick_part != ID_EMPTY)
            {
                if (field_part != ID_EMPTY)
                {
                    // There's already something here
                    m_state = STATE_GAME_ENDING;
                    m_game_over_y = CONST_FIELD_HEIGHT - 1;
                    m_bass->pause();
                    m_bass->play_sfx(SFX_GAME_OVER);
                    m_game_over_start = SDL_GetTicks();
                }
                m_game_field[grid_x + x][grid_y + y] = brick_part;
            }
        }
    }
    auto lines = 0;
    if ((lines = m_current_level->check_line_clears()) > 0)
    {
        if (lines == 4)
            m_bass->play_sfx(SFX_BREAK4);
        else
            m_bass->play_sfx(SFX_BREAK);
        m_state = STATE_LINE_BREAK;
        m_line_clear_start = SDL_GetTicks();
    }
}

void game_logic::handle_events()
{
    while (SDL_PollEvent(m_helper->event()))
    {
        m_helper->handle_events();

        switch (m_helper->event()->type)
        {
        case SDL_QUIT:
            m_run_flag = false;
            m_bass->pause();
            break;
        case SDL_KEYUP:
            if (in_game())
            {
                if (!game_over() && m_state != STATE_LINE_BREAK)
                {
                    switch (m_helper->event()->key.keysym.sym)
                    {
                    case SDLK_e:
                    case SDLK_w:
                    case SDLK_UP:
                        current()->rotate(this);
                        m_bass->play_sfx(SFX_TURN);
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        move_brick(DIR_LEFT);
                        m_bass->play_sfx(SFX_MOVE);
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        move_brick(DIR_RIGHT);
                        m_bass->play_sfx(SFX_MOVE);
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        m_lock_fast_drop = false;
                        break;
                    case SDLK_m:
                        m_bass->toggle_music();
                        break;
                    default: ;
                    }
                }
                else if (game_over())
                {
                    if (m_helper->event()->key.keysym.sym == SDLK_RETURN)
                    {
                        m_state = STATE_IN_MENU;
                        m_bass->switch_music(MUSIC_MENU);
                        m_helper->set_display(153, 138);

                        for (auto& line : m_game_field)
                            for (auto& block : line)
                                block = ID_EMPTY;
                        m_current_level->reset();
                    }
                }
            }
            else if (m_helper->event()->key.keysym.sym == SDLK_RETURN)
            {
                m_score->set_num(0);
                m_level->set_num(0);
                m_lines->set_num(0);
                m_bass->switch_music(MUSIC_A_THEME);
                m_helper->set_display(CONST_DEFAULT_SIDE_BAR_WIDTH + CONST_FIELD_W, CONST_FIELD_H);

                start();
            }
            break;
        case SDL_KEYDOWN:
            if (in_game() && !game_over())
            {
                if (m_helper->event()->key.keysym.sym == SDLK_s && !m_lock_fast_drop)
                {
                    fast_drop();
                }
            }
            break;
        default: ;
        }
    }
}

void game_logic::draw()
{
    m_helper->clear();
    if (in_game())
    {
        auto* c = m_helper->color_bright();
        SDL_SetRenderDrawColor(m_helper->renderer(), c->r, c->g, c->b, c->a);
        SDL_RenderFillRect(m_helper->renderer(), m_helper->field_dim());
        draw_walls();
        draw_sidebar();

        if (m_state == STATE_GAMEOVER)
        {
            draw_gameover();
        }
        else
        {
            const auto origin = m_helper->field_origin();

            for (auto x = 0; x < CONST_FIELD_WIDTH; x++)
            {
                for (auto y = 0; y < CONST_FIELD_HEIGHT; y++)
                {
                    SDL_Point p = {0, 0};
                    switch (m_game_field[x][y])
                    {
                    case ID_I_BRICK_BTM:
                    case ID_I_BRICK_BTM_90:
                    case ID_I_BRICK_MID:
                    case ID_I_BRICK_MID_90:
                    case ID_I_BRICK_TOP:
                    case ID_I_BRICK_TOP_90:
                    case ID_L_BRICK:
                    case ID_J_BRICK:
                    case ID_Z_BRICK:
                    case ID_S_BRICK:
                    case ID_SQUARE_BRICK:
                    case ID_T_BRICK:
                        p = util_shift(origin, x * CONST_BRICK_DIM,
                            y * CONST_BRICK_DIM, m_helper->scale());
                        m_sprites[m_game_field[x][y]].draw_absolute(&p, m_atlas);
                    default:
                        break;
                    }
                }
            }

            if (m_state != STATE_GAME_ENDING)
            {
                current()->draw(m_helper, this);
                next()->draw(m_helper, this);
            }
        }

        if (m_state == STATE_LINE_BREAK)
        {
            if (SDL_GetTicks() % 400 < 200)
            {
                m_current_level->draw_line_clear();
            }
            next()->draw(m_helper, this);
        }
    }
    else
    {
        draw_start();
    }
    m_helper->repaint();
}

void game_logic::do_game_over()
{
    if (m_game_over_y >= 0)
    {
        if (SDL_GetTicks() % 100 < 80)
        {
            for (auto& x : m_game_field)
                x[m_game_over_y] = ID_S_BRICK;

            m_game_over_y--;
        }
    }
}

level* game_logic::lvl() const
{
    return m_current_level;
}

brick* game_logic::current()
{
    return &m_current_brick;
}

brick* game_logic::next()
{
    return &m_next_brick;
}

bass_handler* game_logic::bass() const
{
    return m_bass;
}

sdl_helper* game_logic::helper() const
{
    return m_helper;
}

sprite_id game_logic::get_at(const int x, const int y)
{
    return m_game_field[x][y];
}

void game_logic::set_at(const int x, const int y, const sprite_id id)
{
    m_game_field[x][y] = id;
}

sprite* game_logic::get_sprite(const sprite_id id)
{
    return &m_sprites[id];
}

atlas* game_logic::get_atlas() const
{
    return m_atlas;
}

bool game_logic::run_flag() const
{
    return m_run_flag;
}
