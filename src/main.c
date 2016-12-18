#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

#include "bird.h"

static void finish(int sig);
static void setScreenOptions();
static void setColorPairs();
static void draw(Bird_T* player);
static void drawBg();

static chtype BG_CHAR;
static const int BG_COLOR = 2;
static const clock_t TICK = 0.1 * CLOCKS_PER_SEC;  // Update tick length for the game

int main(int argc, char *argv[])
{
    BG_CHAR = ACS_BLOCK;
    BIRD_CHAR = ACS_BLOCK;

    (void) signal(SIGINT, finish);
    setScreenOptions();

    Bird_T player = {(COLS / 3), (LINES / 2), BIRD_CHAR, BIRD_COLOR};
    clock_t lastTick = 0;

    draw(&player);

    for (;;)
    {
        clock_t currentTick = clock();
        if((currentTick - lastTick) > TICK)
        {
            lastTick = currentTick;
            int c = getch();
            updatePlayer(&player);
            draw(&player);
            // endwin();
            // printf("x: %d y: %d\n", player.xPos, player.yPos);
        }
    }

    finish(0);
}

static void finish(int sig)
{
    endwin();

    exit(0);
}

static void setScreenOptions()
{
    (void) initscr();       // initialize the curses library
    keypad(stdscr, TRUE);   // enable keyboard mapping
    (void) cbreak();        // take input chars one at a time, no waiting for \n
    // (void) nonl();          // tell curses not to do NL->CR/NL on output
    (void) curs_set(0);     // make the cursor invisible
    (void) noecho();        // do not echo input
    timeout(0);             // non-blocking reads for character input

    if (has_colors())
    {
        start_color();
        setColorPairs();
    }
}

static void setColorPairs()
{
    init_pair(1, COLOR_YELLOW,  COLOR_YELLOW);
    init_pair(2, COLOR_BLUE,    COLOR_BLUE);
    init_pair(3, COLOR_YELLOW,  COLOR_BLUE);
    init_pair(4, COLOR_BLUE,    COLOR_BLUE);
    init_pair(5, COLOR_CYAN,    COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(7, COLOR_WHITE,   COLOR_BLUE);
}

static void draw(Bird_T* player)
{
    drawBg();
    drawPlayer(player);
    refresh();
}

static void drawBg()
{
    attrset(COLOR_PAIR(BG_COLOR));

    for(int y=0; y < LINES; y++)
    {
        for(int x=0; x < COLS; x++)
        {
            mvaddch(y, x, BG_CHAR);
        }
    }
}

