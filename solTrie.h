#ifndef SOLTRIE_H
#define SOLTRIE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define STARTING_CAP 1
#include "struct.h"

void addLeaf(Dance *d);
SolTrie* initTrie(Doubly *row);
void addChild(SolTrie *sol, SolTrie *child);
void freeSol(SolTrie *sol);
void incNumSols(SolTrie *sol);

void addMin(SolTrie *sol, Doubly *hcol);
Doubly *getMin(Dance *d, SolTrie *sol);

#endif
