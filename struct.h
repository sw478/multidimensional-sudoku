#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#define BUFSIZE 1000

/*
 * a linked list of heur headers
 * heur headers are linked lists of hcol heurs
 * heur headers hold all heurs with the same number of elements (in matrix) beneath them together
 * each heur is linked to a matrix column header (hcol)
 */
typedef struct heur
{
   /* used for hcol heurs */
   void *hcol;
   struct heur *head, *prev, *next;

   /* used for heur headers */
   int num;
   struct heur *hprev, *hnext;
} Heur;

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
 * drow/dcol for headers are used to keep track of number of elements
 * in their row or col
 * pointers to adjacent nodes and to header nodes
 */
typedef struct doubly
{
   int drow, dcol;
   struct doubly *up, *down, *left, *right, *hcol, *hrow;
   Heur *heur;
} Doubly;

/*
 * doubly linked list used to hold all matrix rows that need to be hidden before
 * the algorithm runs
 */
typedef struct hide
{
   Doubly *xrow;
   struct hide *next, *prev;
} Hide;

/*
 * actually a tree not a trie
 * row: doubly the solTrie maps to
 * child: list of child solTries
 * parent: all children of a solTrie map its parent
 * pointer to the current solTrie
 * ichild: number of children
 * cap: current capacity of children, starts with STARTING_CAP
 * which is 1 and doubles when capacity is reached
 * numSols: number of solutions (leaves) under this portion of the tree
 *
 * a sudoku with one solution would be the typical case, which is why
 * STARTING_CAP is 1 and most of the time doesn't have to grow
 * this would make the the tree look like a list
 *
 * in the Dance struct, there is solRoot, csol, sols, numSols, and solCap;
 * solRoot: typical root node that only serves as an entry point
 * csol: used as a pointer, maintains position in the tree while
 * algorithm X is recursively called
 * sols: a list of all the solTrie leaf nodes of the tree
 * numSols: # of solutions
 * solCap: capacity of soltions, used for a growing list
 */ 
typedef struct solTrie
{
   Doubly *row;
   struct solTrie **child, *parent;
   int ichild, cap, numSols;
} SolTrie;

/*
 * main structure used in this program, passed in arguments a lot
 * rmax & cmax: dimensions of the matrix, defined manually in initMatrixFileSudoku
 * mode: 0 - generate, 1 - solve
 * x & y: dimensions of a single box of a sudoku board (not entire board)
 * numCalls: keeps track of # of times algorithm X is called and used for diagnostics
 * root: entry point
 * xrow & xcol: pointer for navigation
 * init: file pointer for matrix text file (not to be confused with sudoku text file)
 *
 * ilist, initListCap, & initList are used in sewing the matrix together
 * keeps track of all the Doubly nodes during initialization
 *
 * hideRoot: entry point for the doubly linked list
 * heurRoot: entry point for the two dimensional doubly linked list
 */
typedef struct
{
   int rmax, cmax, mode, x, y, numCalls;
   Doubly *root, *xrow, *xcol;
   FILE *init;

   int ilist, initListCap;
   Doubly **initList;

   long int numSols, solCap;
   SolTrie *solRoot, *csol, **sols;

   Hide *hideRoot;
   Heur *heurRoot;
} Dance;

/*
 * contains all relevant information for the input Sudoku board
 * or output board for generation
 * x and y are the dimensions of a single box (not entire board)
 * in the sudoku
 * xy: just x*y
 * z: in the future, higher dimensional sudoku boards can be possible to solve
 * mode: same as in Dance struct
 * in: file pointer for the sudoku text file (not to be confused with the dance text file)
 * other data is optional
 */
typedef struct
{
   int *grid, x, y, xy, z, gridSize;
   int elements, steps, visited, mode;
   FILE *in;
} Sudoku;

#endif
