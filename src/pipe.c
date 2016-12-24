#include "pipe.h"

#include <stdlib.h>
#include <time.h>

void initPipe(Pipe_T* pipe)
{
    PIPE_MAX_GAP_POS = (LINES - 2 - PIPE_MAX_GAP_SIZE);

    srand(clock() % 7027);
    int gapPos = (rand() % (PIPE_MAX_GAP_POS - PIPE_MIN_GAP_POS)) + PIPE_MIN_GAP_POS;
    int gapSize = (rand() % (PIPE_MAX_GAP_SIZE - PIPE_MIN_GAP_SIZE)) + PIPE_MIN_GAP_SIZE;

    pipe->position = COLS;
    pipe->width = PIPE_WIDTH;
    pipe->gapPos = gapPos;
    pipe->gapSize = gapSize;
    pipe->posInBlock = PIPE_BLOCK_WIDTH;
    pipe->topPipe = newwin((pipe->gapPos-1),
                             PIPE_WIDTH,
                             0,
                             pipe->position);
    pipe->bottomPipe = newwin(0,
                                PIPE_WIDTH,
                                (pipe->gapPos + pipe->gapSize + 1),
                                pipe->position);
    pipe->shouldReap = false;

    fillInPipeSegment(pipe->topPipe);
    fillInPipeSegment(pipe->bottomPipe);
}

void updatePipe(Pipe_T* pipe, int speed)
{
    pipe->posInBlock = pipe->posInBlock - speed;
    if(pipe->posInBlock < 0)
    {
        int change = -(pipe->posInBlock);
        pipe->position = pipe->position - (change / PIPE_BLOCK_WIDTH) - 1;
        pipe->posInBlock = PIPE_BLOCK_WIDTH - (change % PIPE_BLOCK_WIDTH); 
    }

    if(pipe->position < (-PIPE_WIDTH))
    {
        cleanupPipe(pipe);
    }
}

void drawPipe(Pipe_T* pipe)
{
    mvwin(pipe->topPipe, 0, pipe->position);
    mvwin(pipe->bottomPipe, (pipe->gapPos + pipe->gapSize + 1), pipe->position);
    wnoutrefresh(pipe->topPipe);
    wnoutrefresh(pipe->bottomPipe);
}

static void fillInPipeSegment(WINDOW* pipeWin)
{
    wattrset(pipeWin, COLOR_PAIR(PIPE_COLOR));

    int max_x, max_y;
    getmaxyx(pipeWin, max_y, max_x);
    for(int y=0; y < max_y; y++)
    {
        for(int x=0; x < max_x; x++)
        {
            mvwaddch(pipeWin, y, x, ACS_BLOCK);
        }
    }
}

static void cleanupPipe(Pipe_T* pipe)
{
    (void) delwin(pipe->topPipe);
    (void) delwin(pipe->bottomPipe);
    pipe->shouldReap = true;
}

