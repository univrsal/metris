#define SDL_MAIN_HANDLED 1

#include "sdl_helper.hpp"
#include "game_logic.hpp"
#include "util.hpp"

int main(int argc, char** argv)
{
    auto* helper = new sdl_helper();
    auto* logic = new game_logic(helper);

    if (!helper->init(CONST_WINDOW_TITLE, CONST_WINDOW_WIDTH, CONST_WINDOW_HEIGHT)
        || !logic->init())
    {
        return -1;
    }

    while (logic->run_flag())
    {
        logic->handle_events();
        logic->run();
        logic->draw();
    }

    delete helper;
    delete logic;

    return 0;
}
