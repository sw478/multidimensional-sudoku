#include "dance.h"

/*
 * general matrix creator that reads in a text file of coordinates
 * file format: (irow, icol) on separate lines
 */
int initMatrix(Dance *d)
{
   char *buf = malloc(BUFSIZE*sizeof(char));
   int irow, icol;
   Doubly *new;
   memset(buf, 0, BUFSIZE*sizeof(char));

   assert(fseek(d->init, 0, SEEK_SET) == 0);

   while(!feof(d->init))
   {
      fgets(buf, BUFSIZE*sizeof(char), d->init);
      if(feof(d->init))
         break;
      assert(2 == sscanf(buf, "%d %d\n", &irow, &icol));
      new = malloc(sizeof(Doubly));
      new->drow = irow;
      new->dcol = icol;
      if(d->ilist >= d->initListCap)
      {
         d->initListCap *= 2;
         d->initList = realloc(d->initList, d->initListCap*sizeof(Doubly*));
      }
      d->initList[d->ilist] = new;
      d->ilist++;
   }
   d->initList = realloc(d->initList, d->ilist*sizeof(Doubly*));

   connectRows(d);
   connectCols(d);

   fclose(d->init);
   free(buf);

   return 0;
}

int compareRows(const void *a, const void *b)
{
   Doubly *ax = *(Doubly**)a, *bx = *(Doubly**)b;
   int arow = ax->drow, brow = bx->drow;

   return arow == brow ? ax->dcol - bx->dcol : arow - brow;
}

int compareCols(const void *a, const void *b)
{
   Doubly *ax = *(Doubly**)a, *bx = *(Doubly**)b;
   int acol = ax->dcol, bcol = bx->dcol;

   return acol == bcol ? ax->drow - bx->drow : acol - bcol;
}

int connectRows(Dance *d)
{
   Doubly *hrow = d->root, *cur, *hnew;
   int irow, i, prev = -1;
   /* coordinates should already be sorted so you don't need this */
   qsort(d->initList, d->ilist, sizeof(Doubly*), compareRows);

   for(i = 0; i < d->ilist; i++)
   {
      cur = d->initList[i];
      irow = cur->drow;
      if(irow != prev)
      {
         hnew = malloc(sizeof(Doubly));
         hnew->drow = irow;
         hnew->dcol = d->cmax;
         hnew->right = hnew->left = hnew;
         hnew->down = hrow->down;
         hnew->up = hrow;
         hnew->hcol = d->root;
         hnew->hrow = hnew;
         hrow->down->up = hnew;
         hrow->down = hnew;
         hrow = hnew;
      }
      cur->right = hrow;
      cur->left = hrow->left;
      cur->hrow = hrow;
      hrow->left->right = cur;
      hrow->left = cur;
      prev = irow;
   }

   return 0;
}

int connectCols(Dance *d)
{
   Doubly *hcol = d->root, *cur, *hnew;
   int icol, i, prev = -1;
   qsort(d->initList, d->ilist, sizeof(Doubly*), compareCols);

   for(i = 0; i < d->ilist; i++)
   {
      cur = d->initList[i];
      icol = cur->dcol;
      if(icol != prev)
      {
         hnew = malloc(sizeof(Doubly));
         hnew->drow = d->rmax;
         hnew->dcol = icol;
         hnew->down = hnew->up = hnew;
         hnew->right = hcol->right;
         hnew->left = hcol;
         hnew->hcol = hnew;
         hnew->hrow = d->root;
         hcol->right->left = hnew;
         hcol->right = hnew;
         hcol = hnew;
      }
      cur->down = hcol;
      cur->up = hcol->up;
      cur->hcol = hcol;
      hcol->drow++;
      hcol->up->down = cur;
      hcol->up = cur;
      prev = icol;
   }

   return 0;
}
