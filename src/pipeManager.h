#ifndef PIPE_MANAGER_H
#define PIPE_MANAGER_H

#include "pipe.h"

#define MAX_PIPE_COUNT 10
#define MAX_PIPE_SEPARATION 200
#define MIN_PIPE_SEPARATION 50

typedef struct PipeManager {
    Pipe_T* pipeList[MAX_PIPE_COUNT];
} PipeManager_T;

void initManager(PipeManager_T* manager);
void initPipes(PipeManager_T* manager);
void updatePipes(PipeManager_T* manager, int speed);
void drawPipes(PipeManager_T* manager);
void checkPipeCollisions(PipeManager_T* manager, Bird_T* player);
int updateScore(PipeManager_T* manager, Bird_T* player);
static void addNewPipe(PipeManager_T* manager);
static bool shouldAddPipe(PipeManager_T* manager);

#endif
