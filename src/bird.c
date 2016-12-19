#include "bird.h"

void updatePlayer(Bird_T* player)
{
    (*player).speed--;
    (*player).posInBlock = (*player).posInBlock + (*player).speed;

    // Check if the player block should move down
    if((*player).speed < 0 && (*player).posInBlock < 0)
    {
        int change = -((*player).posInBlock);
        (*player).yPos = (*player).yPos + (change / BLOCK_HEIGHT) + 1;
        (*player).posInBlock = BLOCK_HEIGHT - (change % BLOCK_HEIGHT); 
    }
    // Check if the player block should move up
    else if((*player).speed > 0 && (*player).posInBlock > BLOCK_HEIGHT)
    {
        int change = (*player).posInBlock - BLOCK_HEIGHT;
        (*player).yPos = (*player).yPos - (change / BLOCK_HEIGHT) - 1;
        (*player).posInBlock = change % BLOCK_HEIGHT; 
    }
    checkBounds(player);
}

void checkBounds(Bird_T* player)
{
    if((*player).yPos < 0)
    {
        (*player).yPos = 0;
    }
    if((*player).yPos > LINES)
    {
        (*player).yPos = LINES;
    }
    if((*player).xPos < 0 )
    {
        (*player).xPos = 0;
    }
    if((*player).xPos > COLS)
    {
        (*player).xPos = COLS;
    }
}

void jumpPlayer(Bird_T* player)
{
    (*player).speed = BLOCK_HEIGHT;
}

void drawPlayer(Bird_T* player)
{
    attrset(COLOR_PAIR((*player).colorPair));
    mvaddch((*player).yPos, (*player).xPos, (*player).dispChar);
}
