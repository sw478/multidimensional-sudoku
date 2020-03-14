#include "dance.h"

/*
 * returns:
 * 0 on sucess
 * 1 if duplicate
 * -1 if failed
 */
int addDoubly(Dance *d, int irow, int icol)
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

   printf("new: [%d %d],", new->drow, new->dcol);
   printf(" up: [%d %d],", new->up->drow, new->up->dcol);
   printf(" down: [%d %d],", new->down->drow, new->down->dcol);
   printf(" left: [%d %d],", new->left->drow, new->left->dcol);
   printf(" right: [%d %d]\n", new->right->drow, new->right->dcol);

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
   addDoubly(d, 3, 4);
   addDoubly(d, 5, 2);
   addDoubly(d, 7, 12);
   addDoubly(d, 1, 8);
   addDoubly(d, 3, 2);
   addDoubly(d, RMAX-1, 0);
   addDoubly(d, 0, CMAX-1);
   addDoubly(d, RMAX-1, CMAX-1);
   printMatrix(d);

   /*freeDance(d);*/
   printMatrix(d);

   free(d->root);
   return 0;
}

void printMatrix(Dance *d)
{
   int prow, pcol;
   Doubly *xrow = d->root->down, *current;

   for(prow = 0; prow < RMAX; prow++)
   {
      printf("%d: ", prow);
      current = xrow->right;
      for(pcol = 0; pcol < CMAX; pcol++)
      {
         if(current->dcol == pcol && current->drow == prow)
         {
            printf("X");
            current = current->right;
         }
         else if((current->right->dcol == pcol && current->right->dcol == pcol)
            || (current->left->dcol == pcol && current->left->dcol == pcol)
            || (current->up->dcol == pcol && current->up->dcol == pcol)
            || (current->down->dcol == pcol && current->down->dcol == pcol))
         {
            printf("O");
         }
         else
            printf("_");
      }
      xrow = xrow->down;
      printf("\n");
   }
   printf("\n");
}

void freeDance(Dance *d)
{
   Doubly *row, *col, *temp;

   for(col = d->root; col != col->right;)
   {
      printf("%d %d\n", col->dcol, col->drow);
      for(row = col; row != row->down;)
      {
         temp = row;
         printf("\t%d %d\t", row->dcol, row->drow);
         temp->down->up = temp->up;
         temp->up->down = temp->down;
         row = row->down;
         printf("%d %d\n", row->dcol, row->drow);
         free(temp);
      }
      temp = col;
      temp->right->left = temp->left;
      temp->left->right = temp->right;
      col = temp->right;
      /*free(temp);*/
   }
}

