#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include "config.h"
#define F_OK 0

#if defined(_WIN32)
#include <io.h>
#elif defined(__APPLE__)
#include <stdio.h>
#elif defined(__linux__)
#include <stdio.h>
#endif


/* for d->problem */
#define DLX_SOLVE 0
#define DLX_GEN 1
#define ENUMERATE 2
#define ZCHAFF_SOLVE_0 3
#define ZCHAFF_SOLVE_1 4
#define ZCHAFF_GEN_0 5
#define ZCHAFF_GEN_1 6

/* for algX and generate */
#define FOUND 1
#define NOT_FOUND 0

/*
 * a two dimensional linked list of heur headers
 * 
 * heur headers are heads of lists that holds all
 * heurs with the same number of elements beneath them together
 *
 * heurs are doubly linked to each column header
 *
 * incHeur and decHeur are the main operations
 * when a doubly under a column header is covered or uncovered, the
 * column header's element count is appropriately incremented or
 * decremented and the heur it's linked to is moved from one heur list
 * to another
 */
typedef struct heur
{
   void *hcol;
   struct heur *prev, *next;

   int num;
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

   /* misc. data */
   /* could probably move to a struct made for headers that contain doubly */
   Heur *heur;
   int rowIsHidden;
   uint16_t *rowLayout;
} Doubly;

/*
 * each hide is associated with a sudoku cell
 * hrows: each cell is associated with a group of matrix rows, formatted
 * as a list of pointers
 * num: the number for the cell (must be known, so only for given numbers when
 * solving a board), and shouldn't change throughout the course of the program
 * filled: whether or not the cell has been filled with the number, and
 * whether or not the rows have been hidden (should be hidden if filled)
 * 1 - filled/hidden, 0 - empty/unhidden
 */
typedef struct hide
{
   Doubly **hrows;
   int num, filled, iSudoku;
   struct hide *next, *prev;
} Hide;

/*
 * when solving an exact cover problem, a set of rows constitute a solution,
 * and algorithm X finds these rows one at a time, but searches for them using
 * a depth first approach, so what you get is a tree with rows as the nodes,
 * and in this case, the struct solTree acts as the node.
 *
 * row: matrix row associated with the solTree
 * child: pointer array of child nodes
 * parent: each node's children's parent node points to this node
 * ichild: child array's next available index
 * cap: current array capacity
 * numSols: number of leaf nodes including and underneath this current solTree
 */
typedef struct solTree
{
   Doubly *row;
   struct solTree **childList, *parent;
   int ichild, cap;
} SolTree;

/*
 * sudoku: number symbols of the sudoku
 * n: # of dimensions
 * dim: container dimensions
 * containerSize: total # of cells in a container, size of a container
 * sudokuSize: total # of cells in sudoku, size of sudoku
 * 
 */
typedef struct
{
   int *sudoku;
   int *dim, n, containerSize, sudokuSize;

   int numClues;
} Sudoku;

typedef struct
{
   /* root of matrix */
   Doubly *root;
   /* matrix dimensions */
   int rmax, cmax;
   /* number of times AlgX is called */
   long int numCalls;
   FILE *matrixFile;

   /* problem specific data */
   Sudoku *s;
   
   /* for secondary columns */
   Doubly *hcol_sec;
   int sec_hcol_index;

   /* for storing solutions */
   SolTree *csol, **sols;
   int numSols, solCap;

   /* for optional pre processed row covering */
   Hide **hideList, *hideRoot;
   int hidden, hideCap, ihide;
   int genNumCalls;

   /* for column heuristic */
   Heur *heurRoot;
   Heur **heurList;
   int maxColElements;

} Dance;

/*
 * Data structs for different user options
 */

typedef struct
{
    FILE *sudokuFile, *solutionFile;

    Sudoku *s;
    Dance *d;
} DLX;

typedef struct
{
    FILE *enumerateFile;

    Sudoku *s;
    Dance *d;
} Enum;

typedef struct
{
    FILE *sudokuFile, *solutionFile;
    FILE *dimacsInputFile, *dimacsOutputFile;

    Sudoku *s;
} ZChaff;


#endif
