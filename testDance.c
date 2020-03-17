#include "testDance.h"

void testAddAllDoubly(Dance *d)
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

void testX(Dance *d, Sudoku *s)
{
   int x, tests;
   srand(time(NULL));

   d->rmax = 729;
   d->cmax = 324;

   for(tests = 0; tests < 1; tests++)
   {
      d->sol = malloc(d->rmax*sizeof(int));
      memset(d->sol, 0, d->rmax*sizeof(int));
      d->isol = 0;
      initRoot(d);
      initSudokuMatrix(d, s);
      freeColumn(d->root);
 
      //printMatrix(d);
      x = algorithmX(d);
      if(x == 0)
         ;//printSolution(d);
      else
         printf("no solution\n");

      free(d->sol);
      freeDance(d);
      free(d->root);
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
   int prow, pcol, matrix[325][730];
   Doubly *xcol = d->root->right, *xrow = d->root->down;

   memset(matrix, 0, sizeof(matrix));
   for(xcol = d->root->right; xcol != d->root; xcol = xcol->right)
   {
      for(xrow = xcol->down; xrow != xcol; xrow = xrow->down)
         matrix[xcol->dcol][xrow->drow] = 1;
      matrix[xrow->dcol][d->rmax] = xrow->drow - d->rmax;
   }

   //printf("\n\nX's:");
   for(pcol = 0; pcol < d->cmax; pcol++);
      //printf("%3d", matrix[pcol][d->rmax]);
   //printf("\n\n    ");
   for(pcol = 0; pcol < d->cmax; pcol++);
      //printf("%3d", pcol);

   for(prow = 0; prow < d->rmax; prow++)
   {
      printf("\n%3d:", prow);
      for(pcol = 0; pcol < d->cmax; pcol++)
      {
         if(matrix[pcol][prow] == 1)
            printf("X");
         else
            printf("_");
      }
   }
   printf("\n\n");
}

int cmp_int(const void* a, const void* b)
{
   return *(int*)a - *(int*)b;
}

void printSolution(Dance *d)
{
   int prow, pcol, matrix[325][730], i;
   Doubly *xcol = d->root->right, *xrow = d->root->down;

   memset(matrix, 0, sizeof(matrix));
   for(xcol = d->root->right; xcol != d->root; xcol = xcol->right)
   {
      for(xrow = xcol->down; xrow != xcol; xrow = xrow->down)
         matrix[xcol->dcol][xrow->drow] = 1;
      matrix[xrow->dcol][d->rmax] = xrow->drow - d->rmax;
   }

   printf("\n\n    ");
   for(pcol = 0; pcol < d->cmax; pcol++)
      printf("%3d", pcol);
   printf("\n");

   qsort(d->sol, d->isol, sizeof(int), cmp_int); 
   for(i = 0; i < d->isol; i++)
   {
      prow = d->sol[i];
      printf("\n%3d: ", prow);
      for(pcol = 0; pcol < d->cmax; pcol++)
      {
         if(matrix[pcol][prow] == 1)
            printf("  O");
         else
            printf("  |");
      }
   }
   printf("\n\n");
}

void printNodeInfo(Doubly *node)
{
   printf("node: [%d %d],", node->drow, node->dcol);
   printf(" up: [%d %d],", node->up->drow, node->up->dcol);
   printf(" down: [%d %d],", node->down->drow, node->down->dcol);
   printf(" left: [%d %d],", node->left->drow, node->left->dcol);
   printf(" right: [%d %d]\n", node->right->drow, node->right->dcol);
}
