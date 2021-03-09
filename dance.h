#ifndef DANCE_H
#define DANCE_H
#include "struct.h"

int algorithmX(Dance *d);
int algorithmX_RandRows(Dance *d);
int algorithmX_SGen(Dance *d);

void selectCandidateRow(Dance *d, Doubly *node);
void coverColRows(Dance *d, Doubly *xrow);
void coverRows(Dance *d, Doubly *xcol);

void unselectCandidateRow(Dance *d, Doubly *node);
void uncoverColRows(Dance *d, Doubly *xrow);
void uncoverRows(Dance *d, Doubly *xcol);

Doubly *nextRow(Doubly *xrow, int *num, int **hitList);

#endif
