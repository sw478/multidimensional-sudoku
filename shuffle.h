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
void transpose(Dance *d);

#endif
