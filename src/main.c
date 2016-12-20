#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <curses.h>

#include "bird.h"
#include "pipe.h"

static void finish(int sig);
static void setScreenOptions();
static void setColorPairs();
static void runGame();
static void initBg();
static void checkCollisions();
static void draw();
static void drawBg();
static void checkForInputs();
static void cleanup();

static chtype BG_CHAR;
static const int BG_COLOR = 2;
static const clock_t TICK = 0.05 * CLOCKS_PER_SEC;  // Tick length for the game

static Bird_T player;
static Pipe_T firstPipe;
WINDOW* rootWin;
WINDOW* bgWindow;

int main(int argc, char *argv[])
{
    (void) signal(SIGINT, finish);
    setScreenOptions();

    for(;;)
    {
        runGame();
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
    rootWin = initscr();       // initialize the curses library
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
    init_pair(1, COLOR_BLACK,   COLOR_YELLOW);  // Bird
    init_pair(2, COLOR_BLUE,    COLOR_BLUE);    // BG
    init_pair(3, COLOR_GREEN,   COLOR_GREEN);   // Pipes
    init_pair(4, COLOR_BLUE,    COLOR_BLUE);
    init_pair(5, COLOR_CYAN,    COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(7, COLOR_WHITE,   COLOR_BLUE);
}

static void runGame()
{
    initBg();
    initPipe(&firstPipe);
    initPlayer(&player);
    draw();

    clock_t lastTick = 0;
    for (;;)
    {
        clock_t currentTick = clock();
        if((currentTick - lastTick) > TICK)
        {
            lastTick = currentTick;
            updatePlayer(&player);
            updatePipe(&firstPipe, 20);
            checkCollisions();
            draw();
        }

        if(player.dead)
        {
            cleanup();
            return;
        }

        checkForInputs();
    }
}

static void initBg()
{
    BG_CHAR = ACS_BLOCK;
    bgWindow = newwin(0, 0, 0, 0);

    wattrset(bgWindow, COLOR_PAIR(BG_COLOR));

    int max_x, max_y;
    getmaxyx(bgWindow, max_y, max_x);
    for(int y=0; y < max_y; y++)
    {
        for(int x=0; x < max_x; x++)
        {
            mvwaddch(bgWindow, y, x, BG_CHAR);
        }
    }
}

static void checkCollisions()
{
    int max_x = COLS;
    int max_y = LINES;
    getmaxyx(rootWin, max_y, max_x);
    if(player.yPos > max_y)
    {
        player.dead = true;
    }

    int top_x = COLS;
    int top_y = 0;
    getmaxyx(firstPipe.topPipe, top_y, top_x);
    if(player.xPos >= firstPipe.position &&
       player.xPos <= (firstPipe.position + firstPipe.width) &&
       player.yPos <= (top_y - 1))
    {
        player.dead = true;
    }

    int bot_x, bot_y;
    getbegyx(firstPipe.bottomPipe, bot_y, bot_x);
    if(player.xPos >= firstPipe.position &&
       player.xPos <= (firstPipe.position + firstPipe.width) &&
       player.yPos >= bot_y)
    {
        player.dead = true;
    }
}

static void draw()
{
    drawBg();
    if(firstPipe.shouldReap == false)
    {
        drawPipe(&firstPipe);
    }
    drawPlayer(&player);
    doupdate();
}

static void drawBg()
{
    touchwin(bgWindow);
    wnoutrefresh(bgWindow);
}

static void checkForInputs()
{
    int c = getch();
    switch(c) {
    case 'q':
        finish(0);
        break;
    case ' ':
        jumpPlayer(&player);
        break;
    default:
        break;
    }
}

static void cleanup()
{
    (void) delwin(player.window);
    (void) delwin(bgWindow);
}

