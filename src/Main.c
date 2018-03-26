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
#include "Entities/Ship.h"

#define IS_RUNNING 1
#define DELAY 30000

struct _ENTITY_ship
{
    int x, y;
    int vel_x, vel_y;
};

void initialize_ncurses();

int main(int argc, char **argv)
{
    SHIP ship;

    /* ncurses setup */
    int max_x, max_y;
    initialize_ncurses();
    getmaxyx(stdscr, max_y, max_x);
 
    /* DISPLAY */
    init_ship(&ship, (max_x / 2) - 1, max_y - 1, 1);

    /* Main game loop */
    while(IS_RUNNING)
    {
        int ch;
        clear();

        draw_ship(&ship);

        ch = getch();
        switch(ch)
        {
            case KEY_LEFT:
                move_ship(&ship, max_x, 'l');
            case KEY_RIGHT:
                move_ship(&ship, max_x, 'r');
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
    curs_set(FALSE); /* don't show cursor */
    keypad(stdscr, TRUE);
}
