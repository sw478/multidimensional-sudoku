#include "aux.h"

void printBoard(int *grid, int x, int y)
{
   int row, col, xy = x*y;

   printf("\n");
   for(row = 0; row < xy; row++)
   {
      if(row % y == 0)
         printf("\n");
      for(col = 0; col < xy; col++)
      {
         if(col % x == 0)
            printf(" ");
         if(grid[row*xy + col] != 0)
            printf(" %2d", grid[row*xy + col]);
         else
            printf(" __");
      }
      printf("\n");
   }
}

void printMatrix(Dance *d)
{
   int pcol = 0, irow, nrow;
   Doubly *xcol, *xrow;

   printColHeaders(d);

   for(xrow = d->root->down; xrow != d->root; xrow = xrow->down)
   {
      nrow = xrow->dcol - d->cmax;
      printf("\n%3d: ", xrow->drow);
      pcol = 0;
      irow = 0;
      for(xcol = xrow->right; irow < nrow; xcol = xcol->right, irow++, pcol++)
      {
         for(; pcol < xcol->dcol; pcol++, printf("_"));
         printf("X");
      }
      for(; pcol < d->cmax; pcol++, printf("_"));
   }
   printf("\n");
}

void printColHeaders(Dance *d)
{
   int pcol = 0;
   Doubly *xcol;

   printf("     ");
   for(pcol = 0; pcol < d->cmax; printf("%d", pcol % 10), pcol++);
   printf("\nX's: ");
   pcol = 0;
   for(xcol = d->root->right; xcol != d->root; xcol = xcol->right, pcol++)
   {
      for(; pcol < xcol->dcol; pcol++, printf("0"));
      printf("%d", xcol->drow - d->rmax);
   }
   for(; pcol < d->cmax; pcol++, printf("0"));
   printf("\n");
}

void printSolutions(Dance *d)
{
   int i;

   for(i = 0; i < d->numSols; i++)
   {
      printf("\n\nsol %d: ", i + 1);
      printSingleSol2(d, d->sols[i]);
   }
   printf("\n\n");
}

void printSingleSol(Dance *d, SolTrie *sol)
{
   SolTrie *cur;
   Doubly *xcol;
   int pcol, cmax = ((Doubly*)(sol->row))->hcol->dcol;

   printf("\n     ");
   for(pcol = 0; pcol < cmax; printf("%d", pcol % 10), pcol++);
   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      printf("\n%3d: ", ((Doubly*)(cur->row))->drow);
      pcol = 0;
      xcol = ((Doubly*)(cur->row))->right;
      for(; xcol != ((Doubly*)(cur->row)); xcol = xcol->right, pcol++)
      {
         for(; pcol < xcol->dcol; pcol++, printf("."));
         printf("X");
      }
      for(; pcol < xcol->dcol; pcol++, printf("."));
   }
   printf("\n");
}

void printSingleSol2(Dance *d, SolTrie *sol)
{
   SolTrie *cur;
   int drow, num, igrid, xy = d->x*d->y, cmax, *grid;
   if(!sol->row)
      return;
   cmax = ((Doubly*)(sol->row))->hcol->dcol;
   grid = calloc(cmax, sizeof(int));

   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      drow = ((Doubly*)cur->row)->drow;
      num = drow % xy;
      igrid = drow / xy;
      grid[igrid] = num + 1;
   }
   printBoard(grid, d->x, d->y);
   free(grid);
}

int saveSolution(Dance *d, Sudoku *s)
{
   SolTrie *cur;
   int num, igrid, rowNum;

   if(d->numSols > 1)
   {
      printf("\n%lu solutions found\n", d->numSols);
      return d->numSols;
   }

   for(cur = d->sols[0]; cur->parent != cur; cur = cur->parent)
   {
      rowNum = ((Doubly*)(cur->row))->drow;
      num = rowNum % s->xy;
      igrid = rowNum / s->xy;
      s->grid[igrid] = num + 1;
   }

   return 0;
}
