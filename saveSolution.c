#include "struct.h"

/*
    for custom exact cover:
    modify/replace this function with your own
    and make your own print fucntions to translate and
    display the set of rows in the solutions found
*/
int saveSolution(Dance *d)
{
   SolTrie *cur;
   int num, igrid, rowNum, xy = d->s->xy, *grid = d->s->grid;

   if(d->numSols > 1)
   {
      printf("\n%lu solutions found\n", d->numSols);
      return d->numSols;
   }
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