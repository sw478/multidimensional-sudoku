#ifndef SHUFFLE_H
#define SHUFFLE_H
#include "struct.h"

void shuffle(Dance *d);
int *shuffledList(int len);
void swap(int *x, int *y);

void relabel(Dance *d);
void shuffleRows(Dance *d);
void shuffleCols(Dance *d);
void shuffleRowBoxes(Dance *d);
void shuffleColBoxes(Dance *d);
void reflect(Dance *d);
void rotate(Dance *d);

void reflectHorizontal(Dance *d);
void reflectVertical(Dance *d);
void reflectDiagonal(Dance *d);
void rotateOnce(Dance *d);

#endif
