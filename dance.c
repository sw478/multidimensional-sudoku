#include "dance.h"

/*
 * returns:
 * 0 on sucess
 * 1 if duplicate
 * -1 if failed
 */
int initDoubly(Dance *d, int irow, int icol)
{
   Doubly *hrow, *hcol, *new;
   assert(d != NULL);
   assert(d->root != NULL);

   hrow = d->root->down;
   while(hrow->drow != irow && hrow->drow != RMAX)
      hrow = hrow->down;
   if(hrow->drow == RMAX)
      return -1;
   while(hrow->right->dcol <= icol)
      hrow = hrow->right;
   if(hrow->dcol == icol)
      return 1;

   hcol = d->root->right;
   while(hcol->dcol != icol && hcol->dcol != CMAX)
      hcol = hcol->right;
   if(hcol->dcol == CMAX)
      return -1;
   while(hcol->down->drow <= irow)
      hcol = hcol->down;
   if(hcol->drow == irow)
      return 1;

   new = malloc(sizeof(Doubly));
   new->drow = irow;
   new->dcol = icol;

   new->right = hrow->right;
   new->left = hrow;
   hrow->right->left = new;
   hrow->right = new;

   new->down = hcol->down;
   new->up = hcol;
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
   for(drow = 0; drow < RMAX; drow++)
   {
      /* adds Doubly below current Doubly */
      new = malloc(sizeof(Doubly));
      new->drow = drow;
      new->dcol = CMAX;

      new->left = new->right = new;
      new->down = current->down;
      new->up = current;
      current->down->up = new;
      current->down = new;

      current = new;
   }

   current = d->root;
   for(dcol = 0; dcol < CMAX; dcol++)
   {
      /* adds Doubly to the right of current Doubly */
      new = malloc(sizeof(Doubly));
      new->drow = RMAX;
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

int initDance(Sudoku *s)
{
   Dance *d = malloc(sizeof(Dance));

   initRoot(d);

   free(d);
   return 0;
}

int initRoot(Dance *d)
{
   assert(d != NULL);

   d->root = malloc(sizeof(Doubly));
   d->root->drow = RMAX;
   d->root->dcol = CMAX;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;

   initHeaders(d);
   printMatrix(d);

   /*testAddAllDoubly(d);*/
   initDoubly(d, 3, 4);
   initDoubly(d, 5, 2);
   initDoubly(d, 7, 12);
   initDoubly(d, 1, 8);
   initDoubly(d, 3, 2);
   initDoubly(d, RMAX-1, 0);
   initDoubly(d, 0, CMAX-1);
   initDoubly(d, RMAX-1, CMAX-1);
   printMatrix(d);

   freeDance(d);
   free(d->root);
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

void coverDoubly(Doubly *temp)
{
   temp->left->right = temp->right;
   temp->right->left = temp->left;
   temp->down->up = temp->up;
   temp->up->down = temp->down;
}

void testAddAllDoubly(Dance *d)
{
   int r, c;

   for(r = 0; r < RMAX; r++)
   {
      for(c = 0; c < CMAX; c++)
         printf("%d ", initDoubly(d, r, c));
      printf("\n");
   }
}

/* won't work if there aren't any headers */
void printMatrix(Dance *d)
{
   int prow, pcol;
   Doubly *xrow = d->root->down, *current;

   assert(d->root != d->root->down && d->root != d->root->right);
   printf("\n   ");
   for(pcol = 0; pcol < CMAX; pcol++)
      printf("%3d", pcol);
   printf("\n");

   for(prow = 0; prow < RMAX; prow++)
   {
      printf("%d: ", prow);
      current = xrow->right;
      for(pcol = 0; pcol < CMAX; pcol++)
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

