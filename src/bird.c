#include "bird.h"

void updatePlayer(Bird_T* player)
{
    (*player).yPos = (*player).yPos + 1;

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

void drawPlayer(Bird_T* player)
{
    attrset(COLOR_PAIR((*player).colorPair));
    mvaddch((*player).yPos, (*player).xPos, (*player).dispChar);
}
