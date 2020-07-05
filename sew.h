#ifndef SEW_H
#define SEW_H
#include "struct.h"

int initMatrix(Dance *d);
int connectRows(Dance *d);
int connectCols(Dance *d);
int compareRows(const void *a, const void *b);
int compareCols(const void *a, const void *b);

#endif
