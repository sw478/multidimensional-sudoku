#ifndef DANCE_H
#define DANCE_H
#include "struct.h"

Doubly *heuristic(Dance *d);
Doubly *randHCol(Dance *d);
int coverRow(Dance *d, Doubly *node);
int coverCol(Dance *d, Doubly *xrow);
int uncoverRow(Dance *d, Doubly *node);
int uncoverCol(Dance *d, Doubly *xrow);
int algorithmX(Dance *d);
Doubly *nextRow(Doubly *xrow, int *num, int **hitList);
int storeSol(Dance *d, Doubly *hcol);
int coverRowHeaders(Dance *d);
int uncoverRowHeaders(Dance *d);

#endif
