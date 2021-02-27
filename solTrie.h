#ifndef SOLTRIE_H
#define SOLTRIE_H
#define STARTING_CAP 1
#include "struct.h"

void addLeaf(Dance *d);
SolTrie* initTrie(Doubly *row);
void addChild(SolTrie *sol, SolTrie *child);
void freeSol(SolTrie *sol);
void incNumSols(SolTrie *sol);

#endif
