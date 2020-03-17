#include "dance.h"

int initSudokuMatrix(Dance *d, Sudoku *s)
{
   int igrid = 0, inum = 0, sr = 0, sc = 0, sb = 0;

   for(igrid = 0; igrid < 81; igrid++)
   {
      sr = igrid / 9;
      sc = igrid % 9;
      sb = (sr / 3)*3 + sc / 3;

      printf("igrid: %2d\t row: %d\tcol: %d\tbox: %d\tnum: %d\n",
         igrid, sr, sc, sb, s->grid[igrid]);

      if(s->grid[igrid] > 0)
         initSudokuMatrixRow(d, s->grid[igrid]-1, igrid, sr, sc, sb);
      else
      {
         for(inum = 0; inum < 9; inum++)
            initSudokuMatrixRow(d, inum, igrid, sr, sc, sb);
      }
   }

   return 0;
}

void initSudokuMatrixRow(Dance *d, int inum, int igrid, int sr, int sc, int sb)
{
   int irow, icol0, icol1, icol2, icol3;

   irow = sr *81 + sc * 9 + inum;
   icol0 = igrid;
   icol1 = inum + sr*9 + 81;
   icol2 = inum + sc*9 + 81 * 2;
   icol3 = inum + sb*9 + 81 * 3;

   initDoubly(d, irow, icol0);
   initDoubly(d, irow, icol1);
   initDoubly(d, irow, icol2);
   initDoubly(d, irow, icol3);
}
