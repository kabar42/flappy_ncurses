#ifndef PIPE_H
#define PIPE_H

#include <curses.h>

static const int PIPE_COLOR = 3; 
static const int PIPE_WIDTH = 10;
static const int PIPE_MIN_GAP_SIZE = 5;
static const int PIPE_MAX_GAP_SIZE = 15;
static const int PIPE_MIN_GAP_POS = 3;
static int PIPE_MAX_GAP_POS;
static const int PIPE_BLOCK_WIDTH = 10;

typedef struct Pipe
{
    int position;
    int width;
    int gapPos;
    int gapSize;
    int posInBlock;
    WINDOW* topPipe;
    WINDOW* bottomPipe;
    bool shouldReap;
} Pipe_T;

void initPipe(Pipe_T* pipe);
void updatePipe(Pipe_T* pipe, int speed);
void drawPipe(Pipe_T* pipe);
static void fillInPipeSegment(WINDOW* pipeWin);
static void cleanupPipe(Pipe_T* pipe);

#endif
