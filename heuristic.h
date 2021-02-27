#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "struct.h"

void initHeurList(Dance *d);
Heur *initHeur(int num);
void incHeur(Heur *heur);
void decHeur(Heur *heur);
void freeHeur(Dance *d);
void freeHeurHead(Heur *head);

#endif
