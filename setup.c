#include "setup.h"
#include "solTree.h"
#include "dance.h"
#include "heuristic.h"
#include "hide.h"

/* initialize values and structures in the dance struct */
int initDance(Dance *d)
{
   if(d->problem == SUDOKU || d->problem == SUDOKU2)
   {
      d->s->xy = d->s->x*d->s->y;
      d->s->gridSize = d->s->xy*d->s->xy;
   }

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
      d->matrixFile = fopen(matrixFile, "r+");
      free(matrixFile);
      return 1;
   }
   d->matrixFile = fopen(matrixFile, "w+");

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
            fprintf(d->matrixFile, "%d %d\n", irow, icol[i]);
      }
   }

   free(matrixFile);
   return 0;
}