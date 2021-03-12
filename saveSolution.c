#include "saveSolution.h"

/*
   translates solution matrix rows to a filled sudoku board

   for custom exact cover:
   modify/replace this function with your own
   and make your own print fucntions to translate and
   display the set of rows in the solutions found
*/
int saveSolution_Sudoku(Dance *d)
{
   SolTree *cur;
   int num, iSudoku, mrow, cSize = d->s->containerSize, *sudoku = d->s->sudoku;

   assert(d->numSols == 1);

   for(cur = d->sols[0]; cur->parent != cur; cur = cur->parent)
   {
      mrow = cur->row->drow;
      num = mrow % cSize;
      iSudoku = mrow / cSize;
      sudoku[iSudoku] = num + 1;
   }

   return 0;
}