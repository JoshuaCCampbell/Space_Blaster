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
#include <ncurses.h>
#include <unistd.h>
#include "Globals.h"
#include "Entities/Ship.h"
#include "Entities/Blaster.h"

#define DELAY 35000

void initialize_ncurses();

int main(int argc, char **argv)
{
    Ship player;
    Blaster player_blaster;
    int is_running = 1;

    /* ncurses setup */
    int max_x, max_y;
    initialize_ncurses();
    getmaxyx(stdscr, max_y, max_x);
 
    init_ship(&player, (max_x / 2) - 1, max_y - 1, 1);
    init_blaster(&player_blaster, 2);

    /* Main game loop */
    while(is_running)
    {
        int ch;
        clear();

        draw_ship(&player);
        if(player_blaster.active == 1)
        {
            draw_blaster(&player_blaster);
            move_blaster(&player_blaster);
        }

        ch = getch();
        switch(ch)
        {
            case KEY_F(1):  /* Quit */
                is_running = 0;
            case KEY_LEFT:
                move_ship(&player, max_x, 'l');
            case KEY_RIGHT:
                move_ship(&player, max_x, 'r'); 
        }
        
        if(ch == KEY_UP)
        {
            player_blaster.active = 1;
            player_blaster.pos_x = player.pos_x + 1;
            player_blaster.pos_y = player.pos_y - 1;
        }
        
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
