#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "bird.h"
#include "pipeManager.h"

static int distanceToNextPipe = MAX_PIPE_SEPARATION; 

void initManager(PipeManager_T* manager)
{
    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
       manager->pipeList[i] = NULL; 
    }
}

void initPipes(PipeManager_T* manager)
{
    addNewPipe(manager);
}

void updatePipes(PipeManager_T* manager, int speed)
{
    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        Pipe_T* curPipe = manager->pipeList[i];

        if(curPipe != NULL)
        {
            if(!curPipe->shouldReap)
            {
                updatePipe(curPipe, speed);
            }

            if(curPipe->shouldReap)
            {
                free(curPipe);
                manager->pipeList[i] = NULL;
            }
        }
    }

    if(shouldAddPipe(manager))
    {
        addNewPipe(manager);
    }
}

void drawPipes(PipeManager_T* manager)
{
    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        Pipe_T* curPipe = manager->pipeList[i];

        if(curPipe != NULL && !curPipe->shouldReap)
        {
            drawPipe(curPipe);
        }
    }
}

void cleanupPipes(PipeManager_T* manager)
{
    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        Pipe_T* curPipe = manager->pipeList[i];

        if(curPipe != NULL)
        {
            free(curPipe);
            manager->pipeList[i] = NULL;
        }
    }
}

void checkPipeCollisions(PipeManager_T* manager, Bird_T* player)
{
    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        Pipe_T* curPipe = manager->pipeList[i];

        if(curPipe != NULL)
        {
            int top_x = COLS;
            int top_y = 0;
            getmaxyx(curPipe->topPipe, top_y, top_x);
            if(player->xPos >= curPipe->position &&
               player->xPos <= (curPipe->position + curPipe->width) &&
               player->yPos <= (top_y - 1))
            {
                player->dead = true;
            }

            int bot_x, bot_y;
            getbegyx(curPipe->bottomPipe, bot_y, bot_x);
            if(player->xPos >= curPipe->position &&
               player->xPos <= (curPipe->position + curPipe->width) &&
               player->yPos >= bot_y)
            {
                player->dead = true;
            }
        }
    }
}

int updateScore(PipeManager_T* manager, Bird_T* player)
{
    int addToScore = 0;

    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        Pipe_T* curPipe = manager->pipeList[i];

        if(manager->pipeList[i] != NULL)
        {
            if(player->xPos > (curPipe->position + curPipe->width) &&
               !curPipe->countedInScore)
            {
                addToScore++;
                curPipe->countedInScore = true;
            }
        }
    }

    return addToScore;
}

static void addNewPipe(PipeManager_T* manager)
{
    Pipe_T* newPipe = malloc(sizeof(Pipe_T));
    initPipe(newPipe);

    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        if(manager->pipeList[i] == NULL)
        {
            manager->pipeList[i] = newPipe;

            srand(clock() % 7027);
            distanceToNextPipe = (rand() % (MAX_PIPE_SEPARATION - MIN_PIPE_SEPARATION)) + MIN_PIPE_SEPARATION;

            if(distanceToNextPipe >= COLS)
            {
                distanceToNextPipe = COLS-1;
            }
            if(distanceToNextPipe <= 0)
            {
                distanceToNextPipe = 0;
            }
            break;
        }
    }
}

static bool shouldAddPipe(PipeManager_T* manager)
{
    int lastPipeX = 0;
    bool shouldAdd = false;

    for(int i=0; i < MAX_PIPE_COUNT; i++)
    {
        if(manager->pipeList[i] != NULL)
        {
            if(manager->pipeList[i]->position > lastPipeX)
            {
                lastPipeX = manager->pipeList[i]->position;
            }
        }
    }

    if((lastPipeX + distanceToNextPipe) < COLS)
    {
        shouldAdd = true;
    }

    return shouldAdd;
}

