#include "dance.h"

int initSudokuMatrix(Dance *d, Sudoku *s)
{
   int igrid = 0, inum = 0, sr = 0, sc = 0, sb = 0;

   for(igrid = 0; igrid < s->gridSize; igrid++)
   {
      sr = igrid / s->xy;
      sc = igrid % s->xy;
      sb = (sr / s->y)*s->y + sc / s->x;

      //numbering doesn't change execution
      //sb = (sc / s->x)*s->x + sr / s->y;

      if(s->grid[igrid] > 0)
         initSudokuMatrixRow(d, s, s->grid[igrid]-1, igrid, sr, sc, sb);
      else
      {
         for(inum = 0; inum < s->xy; inum++)
            initSudokuMatrixRow(d, s, inum, igrid, sr, sc, sb);
      }
   }

   return 0;
}

void initSudokuMatrixRow(Dance *d, Sudoku *s, int inum, int igrid, int sr, int sc, int sb)
{
   int irow, icol0, icol1, icol2, icol3;

   irow = igrid * s->xy + inum;
   icol0 = igrid;
   icol1 = inum + sr*s->xy + s->gridSize;
   icol2 = inum + sc*s->xy + s->gridSize * 2;
   icol3 = inum + sb*s->xy + s->gridSize * 3;

   initDoubly(d, irow, icol0);
   initDoubly(d, irow, icol1);
   initDoubly(d, irow, icol2);
   initDoubly(d, irow, icol3);
}

void saveSolution(Dance *d, Sudoku *s)
{
   int i, num, igrid;

   for(i = 0; i < d->isol; i++)
   {
      num = d->sol[i] % s->xy;
      igrid = d->sol[i] / s->xy;
      s->grid[igrid] = num + 1;
   }
}
