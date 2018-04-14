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
    move(p_blaster->pos_y, p_blaster->pos_x);
    addch('o');
}

void move_blaster(Blaster *p_blaster, int direction)
{
    
    if(direction == 1)
    { 
            p_blaster->pos_y -= p_blaster->vel_y;
    }
    else if(direction == 0)
    {
            p_blaster->pos_y += p_blaster->vel_y;
    }
}
