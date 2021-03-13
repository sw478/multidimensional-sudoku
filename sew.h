#ifndef SEW_H
#define SEW_H
#include "struct.h"

int initMatrix(Dance *d);
int connectRows(Dance *d, Doubly **initList, int ilist);
int connectCols(Dance *d, Doubly **initList, int ilist);
int compareRows(const void *a, const void *b);
int compareCols(const void *a, const void *b);

#endif
