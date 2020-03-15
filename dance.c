#include "dance.h"

int initDance(Sudoku *s)
{
   int res, i;
   Dance *d = malloc(sizeof(Dance));
   d->rmax = 10;
   d->cmax = 10;
   d->isol = 0;
   d->sol = malloc(d->rmax*sizeof(int));

   initRoot(d);

   testAddAllDoubly(d);
   freeColumn(d->root);
   printMatrix(d);

   res = algorithmX(d);
   printf("result: %d\n", res);

   for(i = 0; i < d->isol; i++)
      printf("%d ", d->sol[i]);
   printf("\n");

   freeDance(d);
   free(d->root);
   free(d->sol);
   free(d);
   return 0;
}

int algorithmX(Dance *d)
{
   Doubly *hcol;
   int x;

   printMatrix(d);
   while(d->root != d->root->right)
   {
      hcol = heuristic(d);
      if(hcol->drow == d->rmax)
         return 1;
      cover(d, hcol);
      x = algorithmX(d);
      uncover(d, hcol);
      if(x == 0)
      {
         storeSol(d, hcol);
         printMatrix(d);
         return 0;
      }
   }
   return 0;
}

void storeSol(Dance *d, Doubly *hcol)
{
   Doubly *xcol;

   for(xcol = hcol->down; xcol != hcol; xcol = xcol->down)
   {
      d->sol[d->isol] = xcol->drow;
      d->isol++;
   }
}

int cover(Dance *d, Doubly *hcol)
{
   Doubly *xrow, *xcol = hcol;
   assert(hcol->drow >= d->rmax);

   hcol->right->left = hcol->left;
   hcol->left->right = hcol->right;

   for(xcol = xcol->down; xcol != hcol; xcol = xcol->down)
   {
      for(xrow = xcol->right; xrow != xcol; xrow = xrow->right)
      {
         xrow->up->down = xrow->down;
         xrow->down->up = xrow->up;
         xrow->hcol->drow--;
         if(xrow->hcol->drow == d->rmax)
         {
            xrow->hcol->right->left = xrow->hcol->left;
            xrow->hcol->left->right = xrow->hcol->right;
         }
      }
   }
   return 0;
}

int uncover(Dance *d, Doubly *hcol)
{
   Doubly *xrow, *xcol = hcol;

   hcol->right->left = hcol;
   hcol->left->right = hcol;

   for(xcol = xcol->down; xcol != hcol; xcol = xcol->down)
   {
      for(xrow = xcol->right; xrow != xcol; xrow = xrow->right)
      {
         xrow->up->down = xrow;
         xrow->down->up = xrow;
         if(xrow->hcol->drow == d->rmax)
         {
            xrow->hcol->right->left = xrow->hcol;
            xrow->hcol->left->right = xrow->hcol;
         }
         xrow->hcol->drow++;
      }
   }

   return 0;
}

int initSudokuMatrix(Dance *d)
{
   return 0;
}

/* returns pointer to column header with lowest #'s of ones */
Doubly *heuristic(Dance *d)
{
   Doubly *xcol = d->root->right, *result;

   result = xcol;
   for(; xcol != d->root; xcol = xcol->right)
   {
      if(xcol->drow < result->drow)
         result = xcol;
   }

   return result;
}

void testAddAllDoubly(Dance *d)
{
   int r, c;

   srand(time(NULL));
   for(r = 0; r < d->rmax; r++)
   {
      for(c = 0; c < d->cmax; c++)
      {
         if((rand() % 100) < 80)
            initDoubly(d, r, c);
      }
   }
}

int initRoot(Dance *d)
{
   assert(d != NULL);

   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;

   initHeaders(d);

   return 0;
}

/*
 * returns:
 * 0 on sucess
 * 1 if duplicate
 * -1 if failed
 */
int initDoubly(Dance *d, int irow, int icol)
{
   Doubly *hrow, *hcol, *xcol, *new;
   assert(d != NULL);
   assert(d->root != NULL);

   hrow = d->root->down;
   while(hrow->drow != irow && hrow->drow != d->rmax)
      hrow = hrow->down;
   if(hrow->drow == d->rmax)
      return -1;
   while(hrow->right->dcol <= icol)
      hrow = hrow->right;
   if(hrow->dcol == icol)
      return 1;

   hcol = d->root->right;
   while(hcol->dcol != icol && hcol->dcol != d->cmax)
      hcol = hcol->right;
   if(hcol->dcol == d->cmax)
      return -1;
   xcol = hcol;
   while(hcol->down->drow <= irow)
      hcol = hcol->down;
   if(hcol->drow == irow)
      return 1;

   new = malloc(sizeof(Doubly));
   new->drow = irow;
   new->dcol = icol;
   new->right = hrow->right;
   new->left = hrow;
   new->down = hcol->down;
   new->up = hcol;
   new->hcol = xcol;
   xcol->drow++;

   hrow->right->left = new;
   hrow->right = new;
   hcol->down->up = new;
   hcol->down = new;

   return 0;
}

int initHeaders(Dance *d)
{
   int drow, dcol;
   Doubly *current, *new;
   assert(d != NULL);
   assert(d->root != NULL);

   current = d->root;
   for(drow = 0; drow < d->rmax; drow++)
   {
      /* adds Doubly below current Doubly */
      new = malloc(sizeof(Doubly));
      new->drow = drow;
      new->dcol = d->cmax;

      new->left = new->right = new;
      new->down = current->down;
      new->up = current;
      current->down->up = new;
      current->down = new;

      current = new;
   }

   current = d->root;
   for(dcol = 0; dcol < d->cmax; dcol++)
   {
      /* adds Doubly to the right of current Doubly */
      new = malloc(sizeof(Doubly));
      new->drow = d->rmax;
      new->dcol = dcol;

      new->up = new->down = new;
      new->right = current->right;
      new->left = current;
      current->right->left = new;
      current->right = new;

      current = new;
   }

   return 0;
}

/*frees all nodes in matrix expect for the root node */
void freeDance(Dance *d)
{
   Doubly *col, *temp;

   col = d->root->right;
   while(col != d->root)
   {
      freeColumn(col);
      temp = col;
      col = col->right;
      coverDoubly(temp);
      free(temp);
   }
   freeColumn(col);
}

/* frees all nodes in selected column expect for the input node */
void freeColumn(Doubly *col)
{
   Doubly *row, *temp;

   row = col->down;
   while(row != col)
   {
      temp = row;
      row = row->down;
      coverDoubly(temp);
      free(temp);
   }
}

void coverDoubly(Doubly *node)
{
   node->left->right = node->right;
   node->right->left = node->left;
   node->down->up = node->up;
   node->up->down = node->down;
}

void printMatrix(Dance *d)
{
   int prow, pcol, matrix[100][100];
   Doubly *xcol = d->root->right, *xrow = d->root->down;

   memset(matrix, 0, sizeof(matrix));
   for(xcol = d->root->right; xcol != d->root; xcol = xcol->right)
   {
      for(xrow = xcol->down; xrow != xcol; xrow = xrow->down)
         matrix[xcol->dcol][xrow->drow] = 1;
      matrix[xrow->dcol][0] = xrow->drow - d->rmax;
   }

   printf("\n\n1's:");
   for(pcol = 0; pcol < d->cmax; pcol++)
      printf("%3d", matrix[pcol][0]);
   printf("\n\n    ");
   for(pcol = 0; pcol < d->cmax; pcol++)
      printf("%3d", pcol);

   for(prow = 1; prow < d->rmax; prow++)
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

void printNodeInfo(Doubly *node)
{
   printf("node: [%d %d],", node->drow, node->dcol);
   printf(" up: [%d %d],", node->up->drow, node->up->dcol);
   printf(" down: [%d %d],", node->down->drow, node->down->dcol);
   printf(" left: [%d %d],", node->left->drow, node->left->dcol);
   printf(" right: [%d %d]\n", node->right->drow, node->right->dcol);
}

