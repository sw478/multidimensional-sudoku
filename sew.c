#include "sew.h"

/*
 * general matrix creator that reads in a text file of coordinates
 * file format: (irow, icol) on separate lines
 */
int initMatrix(Dance *d)
{
   char *buf = malloc(BUFSIZE*sizeof(char));
   int irow, icol;
   Doubly *doub;
   memset(buf, 0, BUFSIZE*sizeof(char));

   assert(fseek(d->matrixFile, 0, SEEK_SET) == 0);
   
   /* list of disconnected doubly */
   d->ilist = 0;
   d->initListCap = 1;
   d->initList = malloc(sizeof(Doubly*));

   /* initialize all aspects of root doubly */
   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;
   d->root->hcol = d->root->hrow = d->root;

   while(!feof(d->matrixFile))
   {
      fgets(buf, BUFSIZE*sizeof(char), d->matrixFile);
      if(feof(d->matrixFile))
         break;
      assert(2 == sscanf(buf, "%d %d\n", &irow, &icol));
      doub = malloc(sizeof(Doubly));
      doub->drow = irow;
      doub->dcol = icol;
      if(d->ilist >= d->initListCap)
      {
         d->initListCap = d->initListCap * GROWTH_FACTOR + 1;
         d->initList = realloc(d->initList, d->initListCap*sizeof(Doubly*));
      }
      d->initList[d->ilist] = doub;
      d->ilist++;
   }
   d->initList = realloc(d->initList, d->ilist*sizeof(Doubly*));

   connectRows(d);
   connectCols(d);

   free(d->initList);

   fclose(d->matrixFile);
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
         d->root->drow++;
         prev = irow;
      }
      cur->right = hrow;
      cur->left = hrow->left;
      cur->hrow = hrow;
      hrow->dcol++;
      hrow->left->right = cur;
      hrow->left = cur;
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
         d->root->dcol++;
         prev = icol;
      }
      cur->down = hcol;
      cur->up = hcol->up;
      cur->hcol = hcol;
      hcol->drow++;
      hcol->up->down = cur;
      hcol->up = cur;
   }

   return 0;
}
