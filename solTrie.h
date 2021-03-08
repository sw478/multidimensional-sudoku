#ifndef SOLTRIE_H
#define SOLTRIE_H
#define STARTING_CAP 1
#include "struct.h"

void addLeaf(Dance *d);
SolTrie* initTrie(Doubly *row);
void addChild(SolTrie *sol, SolTrie *child);
void freeTree(SolTrie *sol);
void freeSol(SolTrie *sol);

#endif
