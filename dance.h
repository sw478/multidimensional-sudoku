#ifndef DANCE_H
#define DANCE_H
#include "sudoku.h"

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
   struct doubly *up, *down, *left, *right, *hcol;
} Doubly;

typedef struct
{
   int rmax, cmax, *sol, isol;
   Doubly *root;
} Dance;

/* sudoku specific functions */
int initSudokuMatrix(Dance *d, Sudoku *s);
void initSudokuMatrixRow(Dance *d, int inum, int igrid, int sr, int sc, int sb);
void saveSolution(Dance *d, Sudoku *s);

/* basic operations */
Doubly *heuristic(Dance *d);
int coverRow(Dance *d, Doubly *node);
int coverCol(Dance *d, Doubly *xrow);
int uncoverRow(Dance *d, Doubly *node);
int uncoverCol(Dance *d, Doubly *xrow);
int algorithmX(Dance *d);
int storeSol(Dance *d, Doubly *hcol);

#include "setup.h"
#include "testDance.h"

#endif
