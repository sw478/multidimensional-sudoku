#ifndef DANCE_H
#define DANCE_H
#include "sudoku.h"
#include <unistd.h>
#include "solTrie.h"

typedef struct doubly
{
/* 
 * drow = row * 81 + col * 9 + num
 * dcol = constraint * 81 + row * 9 + col
 * constraint #: 1: row-col, 2: row-num, 3: col-num, 4: box-num
 *
 * if Doubly is a root, drow = d->root->rmax, dcol = d->root->cmax
 * 
 * IMPORTANT: for space saving purposes, if doubly is not header:
 * (drow < d->root->rmax && drow >= 0)
 * if a header: (drow >= d->root->rmax)
 * drow will instead represent: (number of 1's in that column + d->root->rmax)
 */

   int drow, dcol;

/* pointers to nodes adjacent to node, and to the node's column header node */
   struct doubly *up, *down, *left, *right, *hcol, *hrow;
} Doubly;

typedef struct hide
{
   Doubly *xrow;
   struct hide *next, *prev;
} Hide;

typedef struct
{
   int rmax, cmax;
   long int numSols, solCap;
   Doubly *root;
   SolTrie *solRoot, *csol, **sols;
   Hide *hideRoot;
   FILE *init;
} Dance;

typedef struct
{
   Dance **danceList;
} Universal;

/* sudoku specific functions */
int initMatrixFileSudoku(Dance *d, int x, int y);
int initMatrix(Dance *d);
int initSudokuMatrix(Dance *d, Sudoku *s);
int hideRow(Dance *d, Doubly *row);
int recoverHiddenRows(Dance *d);
int saveSolution(Dance *d, Sudoku *s);

/* basic operations */
Doubly *heuristic(Dance *d);
Doubly *randHCol(Dance *d);
int coverRow(Dance *d, Doubly *node);
int coverCol(Dance *d, Doubly *xrow);
int uncoverRow(Dance *d, Doubly *node);
int uncoverCol(Dance *d, Doubly *xrow);
int algorithmX(Dance *d);
int storeSol(Dance *d, Doubly *hcol);
int coverRowHeaders(Dance *d);
int uncoverRowHeaders(Dance *d);

/* Universal operations */
int danceListIndex(int x, int y);

#include "setup.h"
#include "testDance.h"

#endif
