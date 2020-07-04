#include "dance.h"

/*
 * general matrix creator that reads in a text file of coordinates
 * file format: (irow, icol) on separate lines
 */
int initMatrix(Dance *d)
{
   char *buf = malloc(BUFSIZE*sizeof(char));
   int irow, icol;
   memset(buf, 0, BUFSIZE*sizeof(char));

   assert(fseek(d->init, 0, SEEK_SET) == 0);

   while(!feof(d->init))
   {
      fgets(buf, BUFSIZE*sizeof(char), d->init);
      assert(2 == sscanf(buf, "%d %d\n", &irow, &icol));
      initDoubly(d, irow, icol);
      /* change later to sort by row/col and col/row to group and order */
   }
   fclose(d->init);
   free(buf);

   return 0;
}

int initDoubly(Dance *d, int irow, int icol)
{
   Doubly *hrow, *hcol, *xhrow, *xhcol, *temp, *new;
   assert(d != NULL);
   assert(d->root != NULL);
   assert(irow < d->rmax && icol < d->cmax);

   hcol = d->root->right;
   for(; hcol != d->root && hcol->dcol < icol; hcol = hcol->right);
   if(hcol->dcol != icol)
   {
      temp = malloc(sizeof(Doubly));
      temp->dcol = icol;
      temp->drow = d->rmax;
      temp->up = temp->down = temp;
      temp->left = hcol->left;
      temp->right = hcol;
      hcol->left->right = temp;
      hcol->left = temp;
      hcol = temp;
      hcol->hcol = hcol;
   }
   xhcol = hcol;

   hcol = hcol->down;
   for(; hcol->drow != d->rmax && hcol->drow < irow; hcol = hcol->down);
   if(hcol->drow == irow)
      return 1;

   hrow = d->root->down;
   for(; hrow != d->root && hrow->drow < irow; hrow = hrow->down);
   if(hrow->drow != irow)
   {
      temp = malloc(sizeof(Doubly));
      temp->dcol = d->cmax;
      temp->drow = irow;
      temp->left = temp->right = temp;
      temp->down = hrow;
      temp->up = hrow->up;
      hrow->up->down= temp;
      hrow->up = temp;
      hrow = temp;
      hrow->hcol = d->root;
   }
   xhrow = hrow;

   hrow = hrow->right;
   for(; hrow->dcol != d->cmax && hrow->dcol < icol; hrow = hrow->right);
   if(hrow->dcol == icol)
      return 2;

   new = malloc(sizeof(Doubly));
   new->dcol = icol;
   new->drow = irow;

   new->right = hrow;
   new->left = hrow->left;
   new->down = hcol;
   new->up = hcol->up;
   new->hcol = xhcol;
   new->hrow = xhrow;
   new->hcol->drow++;

   hrow->left->right = new;
   hrow->left = new;
   hcol->up->down = new;
   hcol->up = new;

   return 0;
}
