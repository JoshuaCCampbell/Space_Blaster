#include <ncurses.h>
#include "Blaster.h"

void init_blaster(Blaster *p_blaster, int velocity)
{
    p_blaster->pos_x = 0;
    p_blaster->pos_y = 0;
    p_blaster->vel_x = 0;
    p_blaster->vel_y = velocity;
    p_blaster->active = 0;
}

void draw_blaster(Blaster *p_blaster)
{
    mvprintw(p_blaster->pos_y, p_blaster->pos_x, "o");
}

void move_blaster(Blaster *p_blaster)
{
    p_blaster->pos_y -= p_blaster->vel_y;
}
