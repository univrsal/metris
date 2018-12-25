#include "sprite.hpp"
#include "atlas.hpp"

sprite::sprite()
{
    u_v = {};
}

sprite::sprite(uint16_t u, uint16_t v, uint16_t w, uint16_t h)
{
    u_v = {u, v, w, h};
}

void sprite::draw(uint16_t x, uint16_t y, atlas* a)
{
    a->draw(x, y, &u_v);
}

void sprite::draw(const SDL_Point* p, atlas* a)
{
    a->draw(p->x, p->y, &u_v);
}

void sprite::draw_rot(const SDL_Point* p, int rot, atlas* a)
{
    a->draw_rot_nocenter(p->x, p->y, rot, &u_v);
}

void sprite::draw_absolute(const SDL_Point* p, atlas* a)
{
    a->draw_absolute(p->x, p->y, &u_v);
}