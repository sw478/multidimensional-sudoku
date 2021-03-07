#include "struct.h"

/*
    for custom exact cover:
    modify/replace this function with your own
    and make your own print fucntions to translate and
    display the set of rows in the solutions found
*/
int saveSolution_Sudoku(Dance *d)
{
   SolTrie *cur;
   int num, igrid, rowNum, xy = d->s->xy, *grid = d->s->grid;

   if(d->numSols > 1)
      return d->numSols;
   else if(d->numSols == 0)
      return 0;

   for(cur = d->sols[0]; cur->parent != cur; cur = cur->parent)
   {
      rowNum = cur->row->drow;
      num = rowNum % xy;
      igrid = rowNum / xy;
      grid[igrid] = num + 1;
   }

   return 0;
}

/*
    this pairs with initMatrixFileSudoku2()
*/
int saveSolution_Sudoku2(Dance *d)
{
   SolTrie *cur;
   int num, igrid, xy = d->s->xy, *grid = d->s->grid;
   Doubly *hrow, *xrow;

   if(d->numSols > 1)
      return d->numSols;
   else if(d->numSols == 0)
      return 0;
   
   for(cur = d->sols[0]; cur->parent != cur; cur = cur->parent)
   {
      hrow = cur->row;
      num = hrow->drow % xy;

      for(xrow = hrow->right->right; xrow != hrow; xrow = xrow->right)
      {
         igrid = xrow->dcol - xy;
         grid[igrid] = num + 1;
      }
   }

   return 0;
}