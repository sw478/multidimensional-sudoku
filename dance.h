#ifndef DANCE_H
#define DANCE_H
#include "struct.h"

<<<<<<< HEAD
=======
Doubly *heuristic(Dance *d);
Doubly *heuristic2(Dance *d);
>>>>>>> 0c6c7655aeba089eddc4b613790f96950b1a084d
Doubly *randHCol(Dance *d);
int coverRow(Dance *d, Doubly *node);
int coverCol(Dance *d, Doubly *xrow);
int uncoverRow(Dance *d, Doubly *node);
int uncoverCol(Dance *d, Doubly *xrow);
int algorithmX(Dance *d);
Doubly *nextRow(Doubly *xrow, int *num, int **hitList);
int storeSol(Dance *d, Doubly *hcol);

#endif
