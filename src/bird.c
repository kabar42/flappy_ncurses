#include "bird.h"

void initPlayer(Bird_T* player)
{
    player->xPos = (COLS / 4);
    player->yPos = (LINES / 2);
    player->speed = 0;
    player->posInBlock = (BLOCK_HEIGHT / 2);
    player->dispChar = 'W';
    player->colorPair = 1;
    player->dead = false;
    player->window = newwin(1, 1, player->yPos, player->xPos);

    wattrset(player->window, COLOR_PAIR(player->colorPair));
    mvwaddch(player->window, 0, 0, player->dispChar);
}

void updatePlayer(Bird_T* player)
{
    player->speed = player->speed - 2;
    player->posInBlock = player->posInBlock + player->speed;

    // Check if the player block should move down
    if(player->speed < 0 && player->posInBlock < 0)
    {
        int change = -(player->posInBlock);
        player->yPos = player->yPos + (change / BLOCK_HEIGHT) + 1;
        player->posInBlock = BLOCK_HEIGHT - (change % BLOCK_HEIGHT); 
    }
    // Check if the player block should move up
    else if(player->speed > 0 && player->posInBlock > BLOCK_HEIGHT)
    {
        int change = player->posInBlock - BLOCK_HEIGHT;
        player->yPos = player->yPos - (change / BLOCK_HEIGHT) - 1;
        player->posInBlock = change % BLOCK_HEIGHT; 
    }
    checkBounds(player);
}

void jumpPlayer(Bird_T* player)
{
    player->speed = BLOCK_HEIGHT * 1.5;
}

void drawPlayer(Bird_T* player)
{
    mvwin(player->window, player->yPos, player->xPos);
    wnoutrefresh(player->window);
}

static void checkBounds(Bird_T* player)
{
    if(player->yPos < 0)
    {
        player->yPos = 0;
    }
    if(player->xPos < 0 )
    {
        player->xPos = 0;
    }
    if(player->xPos > COLS)
    {
        player->xPos = COLS;
    }
}

