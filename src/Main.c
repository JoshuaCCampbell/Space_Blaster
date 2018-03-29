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
#include "Globals.h"

#define DELAY 30000

void initialize_ncurses();

int main(int argc, char **argv)
{
    Ship player;
    int is_running = 1;

    /* ncurses setup */
    int max_x, max_y;
    initialize_ncurses();
    getmaxyx(stdscr, max_y, max_x);
 
    /* DISPLAY */
    init_ship(&player, (max_x / 2) - 1, max_y - 1, 1);

    /* Main game loop */
    while(is_running)
    {
        int ch;
        clear();

        draw_ship(&player);

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
