#ifndef HIDE_H
#define HIDE_H
#include "struct.h"

int initHide(Dance *d);
void hideAllCells(Dance *d);
int hideSingleCell(Dance *d, Hide *h);
void unhideAllCells(Dance *d);
int unhideSingleCell(Dance *d, Hide *h);

#endif
