#ifndef HEURISTIC_H
#define HEURISTIC_H
#include "struct.h"

<<<<<<< HEAD
Doubly *heuristic(Dance *d);
Doubly *heuristic2(Dance *d);
=======
>>>>>>> 0c6c7655aeba089eddc4b613790f96950b1a084d
void initHeurList(Dance *d, int maxElements);
Heur *initHeur(int num);
void incHeur(Dance *d, Heur *heur, int amount);
void decHeur(Dance *d, Heur *heur, int amount);
void freeHeur(Dance *d);
void freeHeurHead(Heur *head);

#endif
