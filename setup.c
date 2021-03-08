#include "setup.h"
#include "solTree.h"
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

   if(d->problem == SUDOKU || d->problem == SUDOKU2)
   {
      d->s->xy = d->s->x*d->s->y;
      d->s->gridSize = d->s->xy*d->s->xy;
   }

   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;
   d->root->hcol = d->root->hrow = d->root;

   d->numSols = 0;
   d->solCap = STARTING_CAP;
   d->sols = malloc(d->solCap*sizeof(SolTree));

   d->numCalls = 0;

   d->sec_hcol_index = d->cmax; /* default */

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
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   sprintf(matrixFile, "dance/ds1_%dx%d.txt", y, x);

   d->rmax = xy*gridSize;
   d->cmax = 4*gridSize;

   if(access(matrixFile, F_OK) != -1)
   {
      d->init = fopen(matrixFile, "r+");
      free(matrixFile);
      return 1;
   }
   d->init = fopen(matrixFile, "w+");

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

   free(matrixFile);
   return 0;
}

void set_secondary_columns(Dance *d, int index)
{
   d->sec_hcol_index = index;
}

/*
   convert hcols

      root <> ... <> temp2 <> hcol <> ... <> temp1 <> root

   to

      root <> ... <> temp2 <> root
      hcol <> ... <> temp1 <> hcol

   run this before initHeur to prevent heurs on secondary columns
*/
void stitch_secondary(Dance *d)
{
   Doubly *hcol, *temp1, *temp2;

   for(hcol = d->root->right; hcol->dcol != d->sec_hcol_index; hcol = hcol->right);
   d->hcol_sec = hcol;

   temp1 = d->root->left;
   temp2 = hcol->left;

   d->root->left = temp2;
   temp2->right = d->root;

   hcol->left = temp1;
   temp1->right = hcol;
}

void unstitch_secondary(Dance *d)
{
   Doubly *hcol, *temp1, *temp2;

   hcol = d->hcol_sec;

   temp1 = hcol->left;
   temp2 = d->root->left;

   d->root->left = temp1;
   temp1->right = d->root;

   hcol->left = temp2;
   temp2->right = hcol;
}