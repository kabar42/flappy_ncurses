#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <curses.h>

#include "bird.h"
#include "pipeManager.h"

/////////////////////////////////
// Function declarations
/////////////////////////////////
static void finish(int sig);
static void setScreenOptions();
static void setColorPairs();
static void runGame();
static void initBg();
static void initScore();
static void checkCollisions();
static void draw();
static void drawBg();
static void drawScore();
static void checkForInputs();
static void cleanup();

/////////////////////////////////
// Constants
/////////////////////////////////
static chtype BG_CHAR;
static const int BG_COLOR = 2;
static const int SCORE_COLOR = 4;
static const clock_t TICK = 0.05 * CLOCKS_PER_SEC;  // Tick length for the game
static const int SCORE_WIN_LINES = 3;
static const int SCORE_WIN_COLS = 20;

/////////////////////////////////
// Variables
/////////////////////////////////
static int pipeSpeed = 15;
static int score = 0;

static Bird_T player;
static PipeManager_T pipeManager;
WINDOW* rootWin;
WINDOW* bgWindow;
WINDOW* scoreWindow;

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
    init_pair(4, COLOR_WHITE,   COLOR_BLUE);    // Score window
    init_pair(5, COLOR_CYAN,    COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(7, COLOR_WHITE,   COLOR_BLUE);
}

static void runGame()
{
    initBg();
    initManager(&pipeManager);
    initPipes(&pipeManager);
    initPlayer(&player);
    initScore();
    draw();

    clock_t lastTick = 0;
    for (;;)
    {
        clock_t currentTick = clock();
        if((currentTick - lastTick) > TICK)
        {
            lastTick = currentTick;
            updatePlayer(&player);
            updatePipes(&pipeManager, pipeSpeed);
            checkCollisions();
            score += updateScore(&pipeManager, &player);
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

static void initScore()
{
    score = 0;
    scoreWindow = newwin(SCORE_WIN_LINES, SCORE_WIN_COLS, 0, 0);
    wattrset(scoreWindow, COLOR_PAIR(SCORE_COLOR));
    int max_x, max_y;
    getmaxyx(scoreWindow, max_y, max_x);
    for(int y=0; y < max_y; y++)
    {
        for(int x=0; x < max_x; x++)
        {
            mvwaddch(scoreWindow, y, x, ' ');
        }
    }
}

static void checkCollisions()
{
    int max_x = COLS;
    int max_y = LINES;
    getmaxyx(rootWin, max_y, max_x);
    if(player.yPos > max_y)     // player fell past the bottom of the screen
    {
        player.dead = true;
    }

    checkPipeCollisions(&pipeManager, &player);
}

static void draw()
{
    drawBg();
    drawPipes(&pipeManager);
    drawPlayer(&player);
    drawScore();
    doupdate();
}

static void drawBg()
{
    touchwin(bgWindow);
    wnoutrefresh(bgWindow);
}

static void drawScore()
{
    box(scoreWindow, 0, 0);
    char scoreText[15] = "";
    sprintf(scoreText, "Score: %d", score);
    mvwaddstr(scoreWindow, 1, 2, scoreText);
    wnoutrefresh(scoreWindow);
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
    (void) delwin(scoreWindow);
}

