#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
static void showInstructions();
static void showFinalScore();
static void runGame();
static void initBg();
static void initScore();
static void initInstructions();
static void initFinalScore();
static void checkCollisions();
static void draw();
static void drawBg();
static void drawScore();
static void drawInstructions();
static void drawFinalScore();
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
static const int INSTRUCTIONS_WIN_LINES = 10;
static const int INSTRUCTIONS_WIN_COLS = 35;
static const int FINAL_SCORE_WIN_LINES = 8;
static const int FINAL_SCORE_WIN_COLS = 31;

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
WINDOW* instructionsWindow;
WINDOW* finalScoreWindow;

int main(int argc, char *argv[])
{
    (void) signal(SIGINT, finish);
    setScreenOptions();
    showInstructions();

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
    rootWin = initscr();    // initialize the curses library
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

static void showInstructions()
{
    initBg();
    initInstructions();
    drawBg();
    drawInstructions();
    doupdate();

    timeout(-1);

    int ch = wgetch(instructionsWindow);

    (void) delwin(bgWindow);
    (void) delwin(instructionsWindow);

    if(ch == 'q' || ch == 'Q')
    {
        finish(0);
    }

    timeout(0);
}

static void showFinalScore()
{
    initBg();
    initFinalScore();
    drawBg();
    drawFinalScore();
    doupdate();

    timeout(-1);

    int ch = wgetch(finalScoreWindow);

    (void) delwin(bgWindow);
    (void) delwin(finalScoreWindow);

    if(ch == 'q' || ch == 'Q')
    {
        cleanup();
        finish(0);
    }

    timeout(0);
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
            showFinalScore();
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

static void initInstructions()
{
    instructionsWindow = newwin(INSTRUCTIONS_WIN_LINES, 
                                INSTRUCTIONS_WIN_COLS,
                                ((LINES - INSTRUCTIONS_WIN_LINES) / 2),
                                ((COLS - INSTRUCTIONS_WIN_COLS) / 2));
    wattrset(instructionsWindow, COLOR_PAIR(SCORE_COLOR));

    int max_x, max_y;
    getmaxyx(instructionsWindow, max_y, max_x);
    for(int y=0; y < max_y; y++)
    {
        for(int x=0; x < max_x; x++)
        {
            mvwaddch(instructionsWindow, y, x, ' ');
        }
    }
}

static void initFinalScore()
{
    finalScoreWindow = newwin(FINAL_SCORE_WIN_LINES, 
                              FINAL_SCORE_WIN_COLS,
                              ((LINES - FINAL_SCORE_WIN_LINES) / 2),
                              ((COLS - FINAL_SCORE_WIN_COLS) / 2));
    wattrset(finalScoreWindow, COLOR_PAIR(SCORE_COLOR));

    int max_x, max_y;
    getmaxyx(finalScoreWindow, max_y, max_x);
    for(int y=0; y < max_y; y++)
    {
        for(int x=0; x < max_x; x++)
        {
            mvwaddch(finalScoreWindow, y, x, ' ');
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

static void drawInstructions()
{
    int curLine = 1;
    int max_x = COLS;
    int max_y = LINES;
    getmaxyx(instructionsWindow, max_y, max_x);

    box(instructionsWindow, 0, 0);
    const char* title = "INSTRUCTIONS";
    mvwaddstr(instructionsWindow, curLine, ((max_x - strlen(title)) / 2), title);
    curLine += 2;
    const char* jump = "SPACE   -   Jump"; 
    mvwaddstr(instructionsWindow, curLine, 2, jump);
    curLine++;
    const char* quit = "Q       -   Quit"; 
    mvwaddstr(instructionsWindow, curLine, 2, quit);

    curLine = max_y - 2;
    const char* continueStr = "PRESS ANY KEY TO CONTINUE";
    mvwaddstr(instructionsWindow, curLine, ((max_x - strlen(continueStr)) / 2), continueStr);

    wnoutrefresh(instructionsWindow);
}

static void drawFinalScore()
{
    int curLine = 1;
    int max_x = COLS;
    int max_y = LINES;
    getmaxyx(finalScoreWindow, max_y, max_x);

    box(finalScoreWindow, 0, 0);
    const char* title = "SCORE";
    mvwaddstr(finalScoreWindow, curLine, ((max_x - strlen(title)) / 2), title);
    curLine += 2;
    char scoreStr[(FINAL_SCORE_WIN_COLS-4)]; 
    sprintf(scoreStr, "%d", score);
    mvwaddstr(finalScoreWindow, curLine, ((max_x - strlen(scoreStr)) / 2), scoreStr);

    curLine = max_y - 2;
    const char* continueStr = "PRESS ANY KEY TO CONTINUE";
    mvwaddstr(finalScoreWindow, curLine, ((max_x - strlen(continueStr)) / 2), continueStr);

    wnoutrefresh(finalScoreWindow);
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
    (void) delwin(instructionsWindow);
    cleanupPipes(&pipeManager);
}

