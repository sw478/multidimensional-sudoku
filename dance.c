#include "dance.h"

int initDance(Sudoku *s)
{
   Doubly *test;
   Dance *d = malloc(sizeof(Dance));
   d->rmax = 30;
   d->cmax = 30;

   initRoot(d);

   testAddAllDoubly(d);
   printMatrix(d);

   test = heuristic(d);
   printf("col: %d num: %d\n", test->dcol, test->drow - d->rmax);

   freeDance(d);
   free(d->root);
   free(d);
   return 0;
}

int initSudokuMatrix(Dance *d)
{
   return 0;
}

/* returns pointer to column header with lowest #'s of ones */
Doubly *heuristic(Dance *d)
{
   Doubly *result, *xcol = d->root->right;

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
         if((rand() % 100) < 15)
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

/* won't work if there aren't any headers */
void printMatrix(Dance *d)
{
   int prow, pcol;
   Doubly *xcol = d->root->right, *xrow = d->root->down, *current;
   assert(d->root != d->root->down && d->root != d->root->right);

   printf("\n1's:");
   for(; xcol != d->root; xcol = xcol->right)
      printf("%3d", xcol->drow - d->rmax);
   printf("\n\n    ");
   for(pcol = 0; pcol < d->cmax; pcol++)
      printf("%3d", pcol);
   printf("\n");

   for(prow = 0; prow < d->rmax; prow++)
   {
      current = xrow->right;
      printf("%2d: ", prow);
      for(pcol = 0; pcol < d->cmax; pcol++)
      {
         if(current->dcol == pcol && current->drow == prow)
         {
            printf("  X");
            current = current->right;
         }
         else
            printf("  _");
      }
      xrow = xrow->down;
      printf("\n");
   }
   printf("\n");
}

void printNodeInfo(Doubly *node)
{
   printf("node: [%d %d],", node->drow, node->dcol);
   printf(" up: [%d %d],", node->up->drow, node->up->dcol);
   printf(" down: [%d %d],", node->down->drow, node->down->dcol);
   printf(" left: [%d %d],", node->left->drow, node->left->dcol);
   printf(" right: [%d %d]\n", node->right->drow, node->right->dcol);
}

