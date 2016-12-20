#ifndef BIRD_H
#define BIRD_h

#include <curses.h>

static const int BLOCK_HEIGHT = 15;

typedef struct Bird {
    int xPos;
    int yPos;
    int speed;
    int posInBlock;  // Position of the player within the current block / character
    bool dead;
    WINDOW* window;

    chtype dispChar;
    int colorPair;
} Bird_T;

void initPlayer(Bird_T* player);
void updatePlayer(Bird_T* player);
void jumpPlayer(Bird_T* player);
void drawPlayer(Bird_T* player);
static void checkBounds(Bird_T* player);

#endif
