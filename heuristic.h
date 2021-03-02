#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "struct.h"

Doubly *heuristic(Dance *d);
Doubly *heuristic2(Dance *d);
void initHeurList(Dance *d, int maxElements);
Heur *initHeur(int num);
void incHeur(Dance *d, Heur *heur, int amount);
void decHeur(Dance *d, Heur *heur, int amount);
void freeHeur(Dance *d);
void freeHeurHead(Heur *head);

#endif
