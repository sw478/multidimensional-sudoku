#include "testDance.h"

void testAddRandom(Dance *d)
{
   int r, c;

   for(r = 0; r < d->rmax; r++)
   {
      for(c = 0; c < d->cmax; c++)
      {
         if((rand() % 100) < 10)
            initDoubly(d, r, c);
      }
   }
}

void testAddWiki(Dance *d)
{
   initDoubly(d, 0, 0);
   initDoubly(d, 0, 3);
   initDoubly(d, 0, 6);

   initDoubly(d, 1, 0);
   initDoubly(d, 1, 3);

   initDoubly(d, 2, 3);
   initDoubly(d, 2, 4);
   initDoubly(d, 2, 6);

   initDoubly(d, 3, 2);
   initDoubly(d, 3, 4);
   initDoubly(d, 3, 5);

   initDoubly(d, 4, 1);
   initDoubly(d, 4, 2);
   initDoubly(d, 4, 5);
   initDoubly(d, 4, 6);

   initDoubly(d, 5, 1);
   initDoubly(d, 5, 6);
}

void printMatrix(Dance *d)
{
   int pcol = 0;
   Doubly *xcol, *xrow;

   printf("\nX's: ");

   pcol = 0;
   for(xcol = d->root->right; xcol != d->root; xcol = xcol->right, pcol++)
   {
      for(; pcol < xcol->dcol; pcol++, printf("0"));
      printf("%d", xcol->drow - d->rmax);
   }
   for(; pcol < xcol->dcol; pcol++, printf("0"));

   for(xrow = d->root->down; xrow != d->root; xrow = xrow->down)
   {
      if(xrow->right->left != xrow)
         continue;
      printf("\n%3d: ", xrow->drow);
      pcol = 0;
      for(xcol = xrow->right; xcol != xrow; xcol = xcol->right, pcol++)
      {
         for(; pcol < xcol->dcol; pcol++, printf("_"));
         printf("X");
      }
      for(; pcol < xcol->dcol; pcol++, printf("_"));
   }
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
   for(pcol = 0; pcol < cmax; pcol++, printf("%d", pcol % 10));
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
   grid = malloc(cmax*sizeof(int));

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
