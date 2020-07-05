#ifndef DANCE_H
#define DANCE_H
#include "sudoku.h"
#include <unistd.h>
#include "solTrie.h"

/* 
 * columns describe the constraints
 * if Doubly is a root, drow = d->root->rmax, dcol = d->root->cmax
 * 
 * to reduce redundancy, if doubly is not header:
 * 0 < drow < d->root->rmax and
 * 0 < dcol < d->root->cmax
 * if a header:
 * drow >= d->root->rmax
 * dcol >= d->root->cmax
 * drow/dcol for headers will represent number of elements in their row or col
 */
typedef struct doubly
{
   int drow, dcol;

/* pointers to adjacent nodes and to header nodes */
   struct doubly *up, *down, *left, *right, *hcol, *hrow;
} Doubly;

/*
 * in the future when you might want to run multiple boards using a single
 * dancing link structure, this would allow you to not have to recreate one
 */
typedef struct hide
{
   Doubly *xrow;
   struct hide *next, *prev;
} Hide;

typedef struct
{
   int rmax, cmax, mode, x, y, ilist, initListCap;
   long int numSols, solCap;
   Doubly *root, *xrow, *xcol, **initList;
   SolTrie *solRoot, *csol, **sols;
   Doubly *curRow;
   Hide *hideRoot;
   FILE *init;
} Dance;

/* sudoku specific functions */
int initMatrixFileSudoku(Dance *d, int x, int y);
int initMatrix(Dance *d);
int hideRows(Dance *d, Sudoku *s);
int hideRow(Dance *d, Doubly *row);
int recoverHiddenRows(Dance *d);
int saveSolution(Dance *d, Sudoku *s);

/* basic dance moves */
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
void addLeaf(Dance *d);

#include "setup.h"
#include "testDance.h"

#endif
