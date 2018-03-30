/************************************************
* Space Blaster                                 *
*                                               *
* Written by: Joshua Campbell                   *
* Start Date: Mar 26, 2018                      *
*                                               *
* Retro style space shooter game                *
* using ncurses library.                        *
*                                               *
*************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "Globals.h"
#include "Entities/Ship.h"
#include "Entities/Blaster.h"

#define DELAY 35000
#define NUM_PLAYER_SHOTS 5

void initialize_ncurses();

int main(int argc, char **argv)
{
    int i;

    Ship player;
    Blaster player_blaster[5];
    Ship alien;
    int is_running = 1;

    /* ncurses setup */
    int max_x, max_y;
    initialize_ncurses();
    getmaxyx(stdscr, max_y, max_x);
 
    /* Initialize entities */
    init_ship(&player, (max_x / 2) - 1, max_y - 1, 1);
    init_ship(&alien, (max_x / 2) - 1, 5, 1); 
    for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
    {
        init_blaster(&player_blaster[i], 1);
    }

    /* Main game loop */
    while(is_running)
    {
        int ch;
        clear();
        ch = getch();

        /* Draw ships */
        draw_ship(&player, PLAYER_SHAPE);
        draw_ship(&alien, ENEMY_SHAPE);

        /* Check for blaster location */
        for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
        {
            if(player_blaster[i].pos_y < 0)
            {
                player_blaster[i].active = 0;
                player_blaster[i].pos_x = 0;
                player_blaster[i].pos_y = 0;
            }

            if(player_blaster[i].active)
            {
                draw_blaster(&player_blaster[i]);
                move_blaster(&player_blaster[i]);
            }

            /* Blaster hit detection */
            if(
                    player_blaster[i].active && 
                    player_blaster[i].pos_x >= alien.pos_x &&
                    player_blaster[i].pos_x <= (alien.pos_x + 2) &&
                    player_blaster[i].pos_y == alien.pos_y)
            {
                alien.pos_x = 0;
                alien.pos_y = 0;
                player_blaster[i].active = 0;
            }
        } 

        switch(ch)
        {
            case KEY_F(1):  /* Quit */
                is_running = 0;
            case KEY_LEFT:
                move_ship(&player, max_x, 'l');
            case KEY_RIGHT:
                move_ship(&player, max_x, 'r'); 
        }
        
        for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
        {
            if(ch == KEY_UP && player_blaster[i].active == 0)
            {
                player_blaster[i].active = 1;
                player_blaster[i].pos_x = player.pos_x + 1;
                player_blaster[i].pos_y = player.pos_y - 1;
                break;
            }
        }

        /* move_ship(&alien, max_x, 'l'); */
        
        refresh();
        usleep(DELAY);

    }

    endwin();
    exit(EXIT_SUCCESS);
}

void initialize_ncurses()
{
    initscr(); /* init window */
    noecho(); /* don't echo keydowns */
    nodelay(stdscr, TRUE);
    curs_set(FALSE); /* don't show cursor */
    keypad(stdscr, TRUE);
}
