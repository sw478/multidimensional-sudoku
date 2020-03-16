#include "testDance.h"

void testAddAllDoubly(Dance *d)
{
   int r, c;

   for(r = 0; r < d->rmax; r++)
   {
      for(c = 0; c < d->cmax; c++)
      {
         if((rand() % 100) < 30)
            initDoubly(d, r, c);
      }
   }
}

void testX(Dance *d)
{
   int x, tests;
   srand(time(NULL));

   for(tests = 0; tests < 3; tests++)
   {
      d->sol = malloc(d->rmax*sizeof(int));
      memset(d->sol, 0, d->rmax*sizeof(int));
      d->isol = 0;
      initRoot(d);
      testAddAllDoubly(d);
      freeColumn(d->root);
 
      //printMatrix(d);
      x = algorithmX(d);
      if(x == 0)
         printSolution(d);
      printf("x: %d\n", x);

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

   printf("\n\n1's:");
   for(pcol = 0; pcol < d->cmax; pcol++)
      printf("%3d", matrix[pcol][d->rmax]);
   printf("\n\n    ");
   for(pcol = 0; pcol < d->cmax; pcol++)
      printf("%3d", pcol);

   for(prow = 0; prow < d->rmax; prow++)
   {
      printf("\n%2d: ", prow);
      for(pcol = 0; pcol < d->cmax; pcol++)
      {
         if(matrix[pcol][prow] == 1)
            printf("  X");
         else
            printf("  _");
      }
   }
   printf("\n\n");
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

   for(i = 0; i < d->isol; i++)
   {
      prow = d->sol[i];
      printf("\n%2d: ", prow);
      for(pcol = 0; pcol < d->cmax; pcol++)
      {
         if(matrix[pcol][prow] == 1)
            printf("  X");
         else
            printf("   ");
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
