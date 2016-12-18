#ifndef BIRD_H
#define BIRD_h

#include <curses.h>

static chtype BIRD_CHAR;
static const int BIRD_COLOR = 1;

typedef struct Bird {
    int xPos;
    int yPos;
    const chtype dispChar;
    const int colorPair;
} Bird_T;

void updatePlayer(Bird_T* player);
void drawPlayer(Bird_T* player);

#endif
