#ifndef PIPE_H
#define PIPE_H

#include <curses.h>

static const PIPE_COLOR = 3; 
static const PIPE_WIDTH = 5;
static const PIPE_MIN_GAP_SIZE = 5;
static const PIPE_MAX_GAP_SIZE = 15;
static const PIPE_MIN_GAP_POS = 3;
static const PIPE_MAX_GAP_POS = ((LINES-2) - PIPE_MIN_GAP_SIZE);

typedef struct Pipe
{
    int position;
    int width;
    int gapPos;
    int gapSize;
    WINDOW* topPipe;
    WINDOW* bottomPipe;
} Pipe_T;

void initPipe(Pipe_T* pipe);
void updatePipe(Pipe_T* pipe);
void drawPipe(Pipe_T* pipe);

#endif
