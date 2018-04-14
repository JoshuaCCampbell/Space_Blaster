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
void initialize_entities(
            Ship *p_player,
            Blaster *p_playerblaster,
            Ship *p_alien,
            Blaster (*p_alienblaster)[NUM_ALIENS],
            int max_x,
            int max_y
        );
void draw_entities(
            Ship *p_player,
            Blaster *p_playerblaster,
            Ship *p_alien,
            Blaster (*p_alienblaster)[NUM_ALIENS],
            int *p_alivealiens,
            int max_x,
            int max_y
        );
void move_entities(
            Ship *p_player, 
            Blaster *p_playerblaster, 
            Ship *p_alien, 
            Blaster (*p_alienblaster)[NUM_ALIENS],
            int max_x, 
            int max_y,
            int ch, 
            int tick
        );
void hit_detection(Blaster *p_playerblaster, Ship *alien);

int main(int argc, char **argv)
{
    /* Counters */
    int tick = 0;

    /* Declare structs */
    Ship player;
    Blaster player_blaster[NUM_PLAYER_SHOTS];
    Ship alien[NUM_ALIENS];
    Blaster alien_blaster[NUM_ALIEN_SHOTS][NUM_ALIENS];
    int is_running = 1;

    /* ncurses setup */
    int max_x, max_y;
    initialize_ncurses();
    getmaxyx(stdscr, max_y, max_x); 

    /* Game setup */
    initialize_entities(&player, player_blaster, alien, alien_blaster, max_x, max_y);
    
    /* Main game loop */
    while(is_running)
    {
        int ch;
        int alive_aliens = 0;
        ch = getch();
        clear(); /* clear the screen */ 

        /* Move entities */
        move_entities(
                &player, 
                player_blaster, 
                alien, 
                alien_blaster, 
                max_x, 
                max_y, 
                ch, 
                tick);

        /* Draw entities */
        draw_entities(
                &player, 
                player_blaster, 
                alien, 
                alien_blaster, 
                &alive_aliens, 
                max_x, 
                max_y);

        /* Hit detection */
        hit_detection(player_blaster, alien);

        /* Exit on F1 press */       
        if(ch == KEY_F(1))
        {
            is_running = 0;
        }
        
        /* Print number of active aliens */
        mvprintw(1, max_x - 10, "Aliens: %d", alive_aliens); 

        /* Game loop timing */
        refresh();
        tick++;
        if(tick == TICK_BLOCK)
        {
            tick = 0;
        }
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

void initialize_entities(
            Ship *p_player,
            Blaster *p_playerblaster,
            Ship *p_alien,
            Blaster (*p_alienblaster)[NUM_ALIENS],
            int max_x,
            int max_y
        )
{
    int alien_gap = (max_x - 10)  / NUM_ALIENS;
    int i, j;

    /* Init player ship */
    init_ship(p_player, (max_x / 2) - 1, max_y - 1, 0, 1);

    /* Init player blasters */
    for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
    {
        init_blaster(&p_playerblaster[i], 1);
    }

    /* Init alien ships */
    for(i = 0; i < NUM_ALIENS; ++i)
    {
        if(i == 0)
        {
            init_ship(&p_alien[i], 10 , 5, rand() % 2, 1); 
        }
        else
        { 
            init_ship(&p_alien[i], p_alien[i - 1].pos_x + alien_gap, 5, rand() % 2, 1); 
        }
    }

    /* Init alien blaster */
    for(i = 0; i < NUM_ALIENS; ++i)
    {
        for(j = 0; j < NUM_ALIEN_SHOTS; ++j)
        {
            init_blaster(&p_alienblaster[j][i], 1);
        }
    }
}

void draw_entities(
            Ship *p_player,
            Blaster *p_playerblaster,
            Ship *p_alien,
            Blaster (*p_alienblaster)[NUM_ALIENS],
            int *p_alivealiens,
            int max_x,
            int max_y
        )
{
    int i, j;
    
    /* Draw player ship */
    draw_ship(p_player, PLAYER_SHAPE);
    
    /* Draw player blasters */
    for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
    { 
        if(p_playerblaster[i].active)
        {
            draw_blaster(&p_playerblaster[i]);
        }
    }

    /* Draw alien ships */
    for(i = 0; i < NUM_ALIENS; ++i)
    {
        if(p_alien[i].active)
        {
            draw_ship(&p_alien[i], ENEMY_SHAPE);
            ++*p_alivealiens;
        }
    }
    
    /* Draw alien blaster */
    for(i = 0; i < NUM_ALIENS; ++i)
    {
        for(j = 0; j < NUM_ALIEN_SHOTS; ++j)
        {
            if(p_alienblaster[j][i].active)
            {
                draw_blaster(&p_alienblaster[j][i]);
            }
        }
    }
}

void move_entities(
            Ship *p_player, 
            Blaster *p_playerblaster, 
            Ship *p_alien, 
            Blaster (*p_alienblaster)[NUM_ALIENS],
            int max_x, 
            int max_y, 
            int ch,
            int tick
        )
{
    int i, j;

    /* Move player ship */
    switch(ch)
    {
        case KEY_LEFT:
            move_ship(p_player, max_x, 0);
        case KEY_RIGHT:
            move_ship(p_player, max_x, 1); 
    }

    /* Move player blasters */
    for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
    {
        if(p_playerblaster[i].active)
        {
            move_blaster(&p_playerblaster[i], 1);
        }

        /* Clear blaster once it's left the screen */
        if(p_playerblaster[i].pos_y < 0)
        {
            p_playerblaster[i].active = 0;
            p_playerblaster[i].pos_x = 0;
            p_playerblaster[i].pos_y = 0;
        }
        
        /* Set blaster starting point */
        if(ch == KEY_UP && !p_playerblaster[i].active)
        {
            p_playerblaster[i].active = 1;
            p_playerblaster[i].pos_x = p_player->pos_x + 1;
            p_playerblaster[i].pos_y = p_player->pos_y - 1;
            break;
        }
    }

    /* Move alien ships */
    for(i = 0; i < NUM_ALIENS; ++i)
    {
        if(tick % 3 == 0)
        {
            p_alien[i].pos_y++;
        }
        
        if(tick % 6 == 0 && p_alien[i].direction == 0)
        {
            p_alien[i].pos_x--;
        }
        if(tick % 6 == 0 && p_alien[i].direction == 1)
        {
            p_alien[i].pos_x++;
        }

        if(p_alien[i].pos_y >= max_y)
        {
            p_alien[i].pos_y = 0;
            p_alien[i].pos_x = rand() % (max_x - 2);
        }
    }

    /* Move alien blaster */
    for(i = 0; i < NUM_ALIENS; ++i)
    {
        for(j = 0; j < NUM_ALIEN_SHOTS; ++j)
        {
            if(p_alienblaster[j][i].active)
            {
                move_blaster(&p_alienblaster[j][i], 0);
            }

            if(p_alienblaster[j][i].pos_y > max_y)
            {
                p_alienblaster[j][i].active = 0;
                p_alienblaster[j][i].pos_x = 0;
                p_alienblaster[j][i].pos_y = 0;
            }

            if(
                    p_player->pos_x >= p_alien[i].pos_x &&
                    p_player->pos_x <= (p_alien[i].pos_x + 3) &&
                    !p_alienblaster[i][j].active &&
                    p_alien[i].active
                    )
            {
                p_alienblaster[j][i].active = 1;
                p_alienblaster[j][i].pos_x = p_alien[i].pos_x + 1;
                p_alienblaster[j][i].pos_y = p_alien[i].pos_y + 1;
                break;
            } 
        }
    }
}

void hit_detection(Blaster *p_playerblaster, Ship *p_alien)
{
    int i, j;
   
    /* Enemy hit detection for player blaster */ 
    for(i = 0; i < NUM_PLAYER_SHOTS; ++i)
    {
        for(j = 0; j < NUM_ALIENS; ++j)
        {
            if(
                    p_playerblaster[i].active && 
                    p_alien[j].active &&
                    p_playerblaster[i].pos_x >= p_alien[j].pos_x &&
                    p_playerblaster[i].pos_x <= (p_alien[j].pos_x + 2) &&
                    p_playerblaster[i].pos_y <= p_alien[j].pos_y &&
                    p_playerblaster[i].pos_y >= p_alien[j].pos_y - 2)
            {
                p_alien[j].pos_x = 0;
                p_alien[j].pos_y = 0;
                p_alien[j].active = 0;
                p_playerblaster[i].active = 0;
            }
        }
    }
}

