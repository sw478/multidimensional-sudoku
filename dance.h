#ifndef DANCE_H
#define DANCE_H
#include "struct.h"

int algorithmX(Dance *d);
int algorithmX_Gen_Rand(Dance *d);
int algorithmX_Gen_NumSol(Dance *d);

void selectCandidateRow(Dance *d, Doubly *node);
void coverColRows(Dance *d, Doubly *xrow);
void coverRows(Dance *d, Doubly *xcol);

void unselectCandidateRow(Dance *d, Doubly *node);
void uncoverColRows(Dance *d, Doubly *xrow);
void uncoverRows(Dance *d, Doubly *xcol);

Doubly *nextRowRand(Doubly *xrow, int *num, int **hitList);
Doubly *nextRowRand2(Doubly *hcol, Doubly **hitList, int *irand);
Doubly **shuffledList(Dance *d, Doubly *hcol);

#endif
