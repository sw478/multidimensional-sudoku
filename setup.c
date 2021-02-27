#include "setup.h"
#include "solTrie.h"
#include "dance.h"
#include "heuristic.h"
#include "hide.h"

/*
 * initialize values and structures in the dance struct
 *
 * since d->rmax and d->cmax are defined/configured in
 * initMatrixFileSudoku, this has to follow that function
 */
int initDance(Dance *d)
{
   assert(d != NULL);

   d->s->xy = d->s->x*d->s->y;
   d->s->gridSize = d->s->xy*d->s->xy;

   d->ilist = 0;
   d->initListCap = 1;
   d->initList = malloc(sizeof(Doubly*));

   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;
   d->xcol = d->xrow = d->root->hcol = d->root->hrow = d->root;

   d->hideList = malloc(d->s->gridSize*sizeof(Hide*));

   d->solRoot = initTrie(NULL);
   d->csol = d->solRoot->parent = d->solRoot;
   d->numSols = 0;
   d->solCap = 1;
   d->sols = malloc(sizeof(SolTrie));
   d->numCalls = 0;

   return 0;
}

/*
 * creates and writes to a text file coordinates specific
 * to the constrainsts of a general n x m sudoku board with box,
 * row, col constrainsts
 *
 * this function is modular so that it can be substituted by other constraint
 * problems you might want to solve, such as diagonal sudoku boards that have
 * extra diagonal constraints or for an n queens problems or pentomino tiling
 */

/*
 * for sudoku boards:
 * each board size is defined by x and y, where total board size is
 * gridSize = (x*y)^2
 * each row describes a specific position on the board filled with
 * a specific number (0 < num < x*y)
 * drow = (sudoku rows) * (sudoku cols * max num) + col * (max num) + num
 * dcol = constraint * (sudoku rows * sudoku cols) + row * (sudoku cols) + col
 *
 * constraints:
 *    0: row-col
 *    1: row-num
 *    2: col-num
 *    3: box-num
 */
int initMatrixFileSudoku(Dance *d)
{
   int igrid = 0, inum = 0, sr = 0, sc = 0, sb = 0;
   int x = d->s->x, y = d->s->y, xy = d->s->xy, gridSize = d->s->gridSize;
   int irow, icol[4], i;
   char *fileName = malloc(BUFSIZE*sizeof(char));

   sprintf(fileName, "dance/d%dx%d.txt", y, x);

   d->rmax = xy*gridSize;
   d->cmax = 4*gridSize;

   if(access(fileName, F_OK) != -1)
   {
      d->init = fopen(fileName, "r+");
      free(fileName);
      return 1;
   }
   d->init = fopen(fileName, "w+");

   for(igrid = 0; igrid < gridSize; igrid++)
   {
      sr = igrid / xy;
      sc = igrid % xy;
      sb = (sr / y)*y + sc / x;

      for(inum = 0; inum < xy; inum++)
      {
         irow = igrid * xy + inum;
         icol[0] = igrid;
         icol[1] = inum + sr*xy + gridSize;
         icol[2] = inum + sc*xy + gridSize * 2;
         icol[3] = inum + sb*xy + gridSize * 3;

         for(i = 0; i < 4; i++)
            fprintf(d->init, "%d %d\n", irow, icol[i]);
      }
   }

   free(fileName);
   return 0;
}

/*
 * instead of each row describing a single placement of a value and its
 * position, each row will describe the placement of xy number of values
 * (ex. where all the ones are) and no two can overlap, which makes
 * this a valid modification of the first matrix setup
 * 
 * irow: value*layoutNumber (x*y*numLayout)
 * icol: positions on a sudoku board (x*y)^2
 *
 * there are only a set number of possible layouts, and they can be visualized
 * as such:
 *
 * ex. 3x3 board
 * (each a*b corresponds to a x*y box on the suduoku board)
 *
 * 3*3 3*2 3*1
 * 2*3 2*2 2*1
 * 1*3 1*2 1*1
 *
 * this comes out to be: (x!^y)*(y!^x)
 */
int initMatrixFileSudoku2(Dance *d)
{
   int igrid = 0, inum = 0, sr = 0, sc = 0, sb = 0;
   int x = d->s->x, y = d->s->y, xy = d->s->xy, gridSize = d->s->gridSize;
   int irow, icol[4], i;
   char *fileName = malloc(BUFSIZE*sizeof(char));

   sprintf(fileName, "dance/ds2%dx%d.txt", y, x);

   d->rmax = xy*gridSize;
   d->cmax = 4*gridSize;

   if(access(fileName, F_OK) != -1)
   {
      d->init = fopen(fileName, "r+");
      free(fileName);
      return 1;
   }
   d->init = fopen(fileName, "w+");

   numLayouts = xy*factorial(x)*factorial(y)
   numRows = numLayouts*xy

   for(igrid = 0; igrid < gridSize; igrid++)
   {
      sr = igrid / xy;
      sc = igrid % xy;
      sb = (sr / y)*y + sc / x;

      for(inum = 0; inum < xy; inum++)
      {
         irow = igrid * xy + inum;
         icol[0] = igrid;
         icol[1] = inum + sr*xy + gridSize;
         icol[2] = inum + sc*xy + gridSize * 2;
         icol[3] = inum + sb*xy + gridSize * 3;

         for(i = 0; i < 4; i++)
            fprintf(d->init, "%d %d\n", irow, icol[i]);
      }
   }

   free(fileName);
   return 0;
}

/* hardcoded because anything n > 10 is unnecessary */
long int factorial(int n)
{
   if(n < 1 || n > 10)
      factorialError()
}

void coverRowHeaders(Dance *d)
{
   Doubly *hrow;

   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      hrow->right->left = hrow->left;
      hrow->left->right = hrow->right;
   }
}

void uncoverRowHeaders(Dance *d)
{
   Doubly *hrow;

   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      hrow->right->left = hrow;
      hrow->left->right = hrow;
   }
}
