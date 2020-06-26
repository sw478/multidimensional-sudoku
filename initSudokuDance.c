#include "dance.h"

/*
 * initializes text file containing coordinates specific to the constrainsts
 * of a general n x m sudoku board with box, row, col constrainsts
 *
 * modular so that this function can be substituted by any other
 * function writing constraint coordinates for other constraint problems
 * you might want to solve, such as diagonal sudoku boards that have
 * extra diagonal constraints or for an n queens problems or pentomino tiling
 */
int initMatrixFileSudoku(Dance *d, int x, int y)
{
   int igrid = 0, inum = 0, sr = 0, sc = 0, sb = 0, xy = x*y, gridSize = xy*xy;
   int irow, icol[4], i;
   char *buf, *fileName;

   fileName = malloc(BUFSIZE*sizeof(char));
   sprintf(fileName, "d%dx%d.txt", y, x);

   d->init = fopen(fileName, "w+");
   if(0 && d->init != NULL)
   {
      free(fileName);
      return 1;
   }
   buf = malloc(BUFSIZE*sizeof(char));

   for(igrid = 0; igrid < gridSize; igrid++)
   {
      sr = igrid / xy;
      sc = igrid % xy;
      sb = (sr / y)*y + sc / x;

      for(inum = 0; inum < xy; inum++)
      {
         irow = igrid * xy + inum;
         icol[0] = igrid;
         icol[1] = inum + sr*xy + gridSize;
         icol[2] = inum + sc*xy + gridSize * 2;
         icol[3] = inum + sb*xy + gridSize * 3;

         for(i = 0; i < 4; i++)
            fprintf(d->init, "%d %d\n", irow, icol[i]);
      }
   }

   d->rmax = xy*gridSize;
   d->cmax = 4*gridSize;

   free(fileName);
   free(buf);
   return 0;
}

/*
 * general matrix creator that reads in a text file of coordinates
 * format: (irow, icol) on separate lines
 */
int initMatrix(Dance *d)
{
   char buf[BUFSIZE];
   int irow, icol;

   assert(fseek(d->init, 0, SEEK_SET) == 0);
   assert(!feof(d->init));

   initRoot(d);
   while(1)
   {
      fgets(buf, sizeof(buf), d->init);
      if(feof(d->init))
         break;
      assert(2 == sscanf(buf, "%d %d\n", &irow, &icol));
      initDoubly(d, irow, icol);
      /* change later to sort by row/col and col/row to group and order */
   }
   fclose(d->init);

   return 0;
}
/*
 * hides necessary rows and stores them in hideList
 * requires row headers
 */
int initSudokuMatrix(Dance *d, Sudoku *s)
{
   Doubly *row = d->root->down;
   int igrid, num;

   for(row = d->root->down; row != d->root; row = row->down)
   {
      igrid = (int)(row->drow / s->xy);
      num = row->drow % s->xy + 1;
      if(s->grid[igrid] == 0)
         continue;
      if(num == s->grid[igrid])
         continue;
      hideRow(d, row);
   }
   return 0;
}

/*
 * covers/hides a row in a matrix given a Doubly and stores in hideList
 * added to front of hideList
 *
 * done one row at a time
 *
 * 1 Hide is malloced here
 */
int hideRow(Dance *d, Doubly *row)
{
   Hide *hideRow = malloc(sizeof(Hide));
   Doubly *xrow;

   hideRow->next = d->hideRoot->next;
   hideRow->prev = d->hideRoot;
   d->hideRoot->next->prev = hideRow;
   d->hideRoot->next = hideRow;

   hideRow->xrow = row;
   row->up->down = row->down;
   row->down->up = row->up;
   for(xrow = row->right; xrow != row; xrow = xrow->right)
   {
      xrow->up->down = xrow->down;
      xrow->down->up = xrow->up;
      xrow->hcol->drow--;
   }

   return 0;
}

/* 
 * recovers and frees all rows in hideList
 */
int recoverHiddenRows(Dance *d)
{
   Hide *temp, *hideRow = d->hideRoot->next;
   Doubly *xrow;

   while(hideRow != d->hideRoot)
   {
      xrow = hideRow->xrow->right;
      while(xrow != hideRow->xrow)
      {
         xrow->up->down = xrow;
         xrow->down->up = xrow;
         xrow->hcol->drow++;
         xrow = xrow->right;
      }
      xrow->up->down = xrow;
      xrow->down->up = xrow;

      temp = hideRow;
      hideRow->prev->next = hideRow->next;
      hideRow->next->prev = hideRow->prev;
      hideRow = hideRow->next;
      free(temp);
   }
   assert(hideRow->next == hideRow);
   assert(hideRow->prev == hideRow);

   return 0;
}

int saveSolution(Dance *d, Sudoku *s)
{
   SolTrie *cur;
   int num, igrid, rowNum;

   if(d->numSols > 1)
   {
      printf("\n%lu solutions found\n", d->numSols);
      //return d->numSols;
   }

   for(cur = d->sols[0]; cur->parent != cur; cur = cur->parent)
   {
      rowNum = ((Doubly*)(cur->row))->drow;
      num = rowNum % s->xy;
      igrid = rowNum / s->xy;
      s->grid[igrid] = num + 1;
   }

   return 0;
}
