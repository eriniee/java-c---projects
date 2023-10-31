#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

struct rect{
    int row;
    int col;
    int width;
    int height;
};

struct ball {
    int row;
    int col;
    int size;
    int velRow;
    int velCol;
};

int rectCollision(struct rect, struct ball);


#endif