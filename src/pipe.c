#include "pipe.h"

#include <stdlib.h>
#include <time.h>

void initPipe(Pipe_T* pipe)
{
    PIPE_MAX_GAP_POS = ((LINES-2) - PIPE_MIN_GAP_SIZE);

    srand(clock() % 7027);
    int gapPos = (rand() % (PIPE_MAX_GAP_POS - PIPE_MIN_GAP_POS)) + PIPE_MIN_GAP_POS;
    int gapSize = (rand() % (PIPE_MAX_GAP_SIZE - PIPE_MIN_GAP_SIZE)) + PIPE_MIN_GAP_SIZE;

    (*pipe).position = COLS;
    (*pipe).width = PIPE_WIDTH;
    (*pipe).gapPos = gapPos;
    (*pipe).gapSize = gapSize;
    (*pipe).topPipe = newwin(((*pipe).gapPos-1),
                             PIPE_WIDTH,
                             0,
                             (*pipe).position);
    (*pipe).bottomPipe = newwin((LINES - ((*pipe).gapPos + (*pipe).gapSize)),
                                PIPE_WIDTH,
                                (((*pipe).gapPos + (*pipe).gapSize) + 1),
                                (*pipe).position);

}

void updatePipe(Pipe_T* pipe)
{
}

void drawPipe(Pipe_T* pipe)
{
}
