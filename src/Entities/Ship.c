#include <ncurses.h>
#include "Ship.h"

#define SHIP_WIDTH 3

/* Initialize ship */
void init_ship(Ship *p_ship, int start_x, int start_y, int velocity)
{
    p_ship->pos_x = start_x;
    p_ship->pos_y = start_y;
    p_ship->vel_x = velocity;
    p_ship->vel_y = velocity;
}

/* Draw ship */
void draw_ship(Ship *p_ship)
{
    mvprintw(p_ship->pos_y, p_ship->pos_x, "-^-"); /* Check width */
}

/* Set location for ship */
void move_ship(Ship *p_ship, int max_x, char direction){
    if(direction == 'l')
    {
        p_ship->pos_x -= 2;
    }
    else if(direction == 'r')
    {
       p_ship->pos_x++;
    }

    if(p_ship->pos_x < 0)
    {
        p_ship->pos_x = max_x - SHIP_WIDTH - 1;
    }
    else if(p_ship->pos_x + SHIP_WIDTH > max_x)
    {
        p_ship->pos_x = 0;
    }
}

