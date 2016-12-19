#ifndef BIRD_H
#define BIRD_h

#include <curses.h>

static chtype BIRD_CHAR;
static const int BIRD_COLOR = 1;
static const int BLOCK_HEIGHT = 15;

typedef int speed_t;

typedef struct Bird {
    int xPos;
    int yPos;
    speed_t speed;
    int posInBlock;  // Position of the player within the current block / character

    const chtype dispChar;
    const int colorPair;
} Bird_T;

void updatePlayer(Bird_T* player);
void checkBounds(Bird_T* player);
void jumpPlayer(Bird_T* player);
void drawPlayer(Bird_T* player);

#endif
