#include "sew.h"
#include "matrixFile.h"

/*
 * general matrix creator that reads in a text file of coordinates
 * file format: (irow, icol) on separate lines
 */
int initMatrix(Dance *d)
{
   char *buf = malloc(BUFSIZE*sizeof(char));
   char *matrixFileName = getMatrixFileName(d);
   int irow, icol, ilist, initListCap;
   Doubly *doub, **initList;

   d->matrixFile = fopen(matrixFileName, "r+");
   free(matrixFileName);
   assert(d->matrixFile);
   assert(fseek(d->matrixFile, 0, SEEK_SET) == 0);
   memset(buf, 0, BUFSIZE*sizeof(char));

   ilist = 0;
   initListCap = 1;
   initList = malloc(sizeof(Doubly*));

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
      sscanf(buf, "%d %d\n", &irow, &icol);
      //assert(2 == sscanf(buf, "%d %d\n", &irow, &icol));
      doub = malloc(sizeof(Doubly));
      doub->drow = irow;
      doub->dcol = icol;
      if(ilist >= initListCap)
      {
         initListCap = initListCap * GROWTH_FACTOR + 1;
         initList = realloc(initList, initListCap*sizeof(Doubly*));
      }
      initList[ilist] = doub;
      ilist++;
   }
   initList = realloc(initList, ilist*sizeof(Doubly*));

   connectRows(d, initList, ilist);
   connectCols(d, initList, ilist);

   free(initList);

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

int connectRows(Dance *d, Doubly **initList, int ilist)
{
   Doubly *hrow = d->root, *cur, *hnew, *root = d->root;
   int irow, i, prev = -1, cmax = d->cmax;
   qsort(initList, ilist, sizeof(Doubly*), compareRows);

   for(i = 0; i < ilist; i++)
   {
      cur = initList[i];
      irow = cur->drow;
      if(irow != prev)
      {
         hnew = malloc(sizeof(Doubly));
         hnew->drow = irow;
         hnew->dcol = cmax;
         hnew->right = hnew->left = hnew;
         hnew->down = hrow->down;
         hnew->up = hrow;
         hnew->hcol = root;
         hnew->hrow = hnew;
         hrow->down->up = hnew;
         hrow->down = hnew;
         hrow = hnew;
         root->drow++;
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

int connectCols(Dance *d, Doubly **initList, int ilist)
{
   Doubly *hcol = d->root, *cur, *hnew, *root = d->root;
   int icol, i, prev = -1, rmax = d->rmax;
   qsort(initList, ilist, sizeof(Doubly*), compareCols);

   for(i = 0; i < ilist; i++)
   {
      cur = initList[i];
      icol = cur->dcol;
      if(icol != prev)
      {
         hnew = malloc(sizeof(Doubly));
         hnew->drow = rmax;
         hnew->dcol = icol;
         hnew->down = hnew->up = hnew;
         hnew->right = hcol->right;
         hnew->left = hcol;
         hnew->hcol = hnew;
         hnew->hrow = root;
         hcol->right->left = hnew;
         hcol->right = hnew;
         hcol = hnew;
         root->dcol++;
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
