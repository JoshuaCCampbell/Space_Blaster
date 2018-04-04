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

void initialize_ncurses();

int main(int argc, char **argv)
{
    /* Counters */
    int i;
    int j;

    /* Declare structs */
    Ship player;
    Blaster player_blaster[NUM_PLAYER_SHOTS];
    Ship alien[NUM_ALIENS];
    int is_running = 1;

    /* ncurses setup */
    int max_x, max_y;
    initialize_ncurses();
    getmaxyx(stdscr, max_y, max_x);

    int alien_gap = (max_x - 10)  / NUM_ALIENS;
 
    /* Initialize entities */
    /****************************************************************/
    init_ship(&player, (max_x / 2) - 1, max_y - 1, 1);

    for(i = 0; i < NUM_ALIENS; ++i)
    {
        if(i == 0)
        {
            init_ship(&alien[i], 10 , 5, 1); 
        }
        else
        { 
            init_ship(&alien[i], alien[i - 1].pos_x + alien_gap, 5, 1); 
        }
    }

    for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
    {
        init_blaster(&player_blaster[i], 1);
    }
    /****************************************************************/
    
    /* Main game loop */
    while(is_running)
    {
        int ch;
        clear();
        ch = getch();

        /* Draw player ship */
        draw_ship(&player, PLAYER_SHAPE);

        /* Draw alien ship */
        for(i = 0; i < NUM_ALIENS; ++i)
        {
            if(alien[i].active)
            {
                draw_ship(&alien[i], ENEMY_SHAPE);
            }
        }

        /* Player blaster loop */
        for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
        {
            /* Clear blaster once it's left the screen */
            if(player_blaster[i].pos_y < 0)
            {
                player_blaster[i].active = 0;
                player_blaster[i].pos_x = 0;
                player_blaster[i].pos_y = 0;
            }

            /* Draw blaster */
            if(player_blaster[i].active)
            {
                draw_blaster(&player_blaster[i]);
                move_blaster(&player_blaster[i]);
            }

            /* Blaster hit detection */
            for(j = 0; j < NUM_ALIENS; ++j)
            {
                if(
                        player_blaster[i].active && 
                        alien[j].active &&
                        player_blaster[i].pos_x >= alien[j].pos_x &&
                        player_blaster[i].pos_x <= (alien[j].pos_x + 2) &&
                        player_blaster[i].pos_y == alien[j].pos_y)
                {
                    alien[j].pos_x = 0;
                    alien[j].pos_y = 0;
                    alien[j].active = 0;
                    player_blaster[i].active = 0;
                }
            }
        } 

        /* Movement keys and exit */
        switch(ch)
        {
            case KEY_F(1):  /* Quit */
                is_running = 0;
            case KEY_LEFT:
                move_ship(&player, max_x, 'l');
            case KEY_RIGHT:
                move_ship(&player, max_x, 'r'); 
        }
        
        /* Shoot blaster */
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

        /* Move enemies here */
        
        refresh();
        usleep(DELAY);
    }

    /* Exit procedures */
    endwin();
    exit(EXIT_SUCCESS);
}

void initialize_ncurses()
{
    initscr(); /* Init window */
    noecho(); /* Don't echo keydowns */
    nodelay(stdscr, TRUE); /* Don't wait for input for obv reasons */
    curs_set(FALSE); /* Don't show cursor */
    keypad(stdscr, TRUE); /* Allow special  chars */
}

