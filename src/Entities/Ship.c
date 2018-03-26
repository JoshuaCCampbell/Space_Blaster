#include <ncurses.h>
#include "Ship.h"

#define SHIP_WIDTH 3

struct _ENTITY_ship
{
    int x, y;
    int vel_x, vel_y;
};

/* Initialize ship */
void init_ship(SHIP * p_ship, int start_x, int start_y, int velocity)
{
    p_ship->x = start_x;
    p_ship->y = start_y;
    p_ship->vel_x = velocity;
    p_ship->vel_y = velocity;
}

/* Draw ship */
void draw_ship(SHIP * p_ship)
{
    mvprintw(p_ship->y, p_ship->x, "-^-"); /* Don't forget to check SHIP_WIDTH */
}

/* Set location for ship */
void move_ship(SHIP *p_ship, int max_x, char direction){
    if(direction == 'l')
    {
        p_ship->x -= 2;
    }
    else if(direction == 'r')
    {
       p_ship->x++;
    }

    if(p_ship->x < 0)
    {
        p_ship->x = max_x - SHIP_WIDTH - 1;
    }
    else if(p_ship->x + SHIP_WIDTH > max_x)
    {
        p_ship->x = 0;
    }
}

