#include "auxil.h"

void printBoard(Dance *d, int *grid)
{
   int x = d->s->x, y = d->s->y;
   int row, col, xy = x*y;

   for(row = 0; row < xy; row++)
   {
      if(row % y == 0)
         printf("\n");
      for(col = 0; col < xy; col++)
      {
         if(col % x == 0)
            printf(" ");
         if(grid[row*xy + col] != 0)
            printf(" %2d", grid[row*xy + col]);
         else
            printf(" __");
      }
      printf("\n");
   }
   printf("\n");
}

void printMatrix(Dance *d)
{
   int pcol = 0, irow, nrow;
   Doubly *xcol, *xrow;

   //printColHeaders(d);

   for(xrow = d->root->down; xrow != d->root; xrow = xrow->down)
   {
      nrow = xrow->dcol - d->cmax;
      if(nrow == 0)
         continue;
      printf("%3d: ", xrow->drow);
      pcol = 0;
      irow = 0;
      xcol = xrow->right;
      for(; irow < nrow; xcol = xcol->right, irow++, pcol++)
      {
         for(; pcol < xcol->dcol; pcol++, printf("."));
         printf("X");
      }
      for(; pcol < d->cmax; pcol++, printf("."));
      printf("\n");
   }
   printf("\n");
}

void printColHeaders(Dance *d)
{
   int pcol = 0;
   Doubly *xcol;

   printf("     ");
   for(pcol = 0; pcol < d->cmax; printf("%d", pcol % 10), pcol++);
   printf("\nX's: ");
   pcol = 0;
   for(xcol = d->root->right; xcol != d->root; xcol = xcol->right, pcol++)
   {
      for(; pcol < xcol->dcol; pcol++, printf("0"));
      printf("%d", xcol->drow - d->rmax);
   }
   for(; pcol < d->cmax; pcol++, printf("0"));
   printf("\n");
}

/*
   of course, won't work if matrix isn't solving a sudoku
*/
void printSolutions_Sudoku(Dance *d)
{
   int i;

   if(d->numSols != 1)
   {
      printf("\n%lu solutions found\n", d->numSols);
      return;
   }

   for(i = 0; i < d->numSols; i++)
   {
      printf("\nsol %d: \n", i + 1);
      //printSingleSol_Matrix(d, d->sols[i]); /* prints rows of matrices */
      printSingleSol_Sudoku(d, d->sols[i]); /* prints the solution as a sudoku grid */
   }
   printf("\n");
}

void printSolutions_Sudoku2(Dance *d)
{
   int i;

   if(d->numSols != 1)
   {
      printf("\n%lu solutions found\n", d->numSols);
      return;
   }
   
   for(i = 0; i < d->numSols; i++)
   {
      printf("\nsol %d: \n", i + 1);
      //printSingleSol_Matrix(d, d->sols[i]); /* prints rows of matrices */
      printSingleSol_Sudoku2(d, d->sols[i]);
   }
   printf("\n");
}

void printSingleSol_Matrix(Dance *d, SolTrie *sol)
{
   SolTrie *cur;
   Doubly *xcol;
   int pcol, irow;
   if(sol == d->solRoot)
      return;

   //printColHeaders(d);
   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      printf("%3d: ", cur->row->drow);
      pcol = 0;
      irow = 0;
      xcol = cur->row->right;
      for(; pcol < cur->row->left->dcol; xcol = xcol->right, irow++, pcol++)
      {
         for(; pcol < xcol->dcol; pcol++, printf("."));
         printf("X");
      }
      for(; pcol < d->cmax; pcol++, printf("."));
      printf("\n");
   }
   printf("\n");
}

void printSingleSol_Sudoku(Dance *d, SolTrie *sol)
{
   SolTrie *cur;
   int drow, num, igrid, xy = d->s->xy, *grid;
   if(!sol->row)
      return;
   grid = calloc(d->cmax, sizeof(int));

   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      drow = cur->row->drow;
      num = drow % xy;
      igrid = drow / xy;
      grid[igrid] = num + 1;
   }
   printBoard(d, grid);
   free(grid);
}

void printSingleSol_Sudoku2(Dance *d, SolTrie *sol)
{
   SolTrie *cur;
   int num, igrid, xy = d->s->xy, *grid;
   Doubly *hrow, *xrow;

   if(!sol->row)
      return;
   grid = calloc(d->cmax, sizeof(int));

   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      hrow = cur->row;
      num = hrow->drow % xy;

      for(xrow = hrow->right->right; xrow != hrow; xrow = xrow->right)
      {
         igrid = xrow->dcol - xy;
         grid[igrid] = num + 1;
      }
   }
   printBoard(d, grid);
   free(grid);
}

void printHeur(Dance *d)
{
   Heur *head = d->heurRoot, *heur;
   Doubly *hcol;

   printf("heuristics: \n");

   if(head->next != head)
   {
      printf("r: ");
      for(heur = head->next; heur != head; heur = heur->next)
      {
         hcol = ((Doubly*)heur->hcol);
         printf("%d ", hcol->dcol);
      }
      printf("\n");
   }

   for(head = d->heurRoot->hnext; head != d->heurRoot; head = head->hnext)
   {
      if(head->next == head)
         continue;
      printf("%d: ", head->num);
      for(heur = head->next; heur != head; heur = heur->next)
      {
         hcol = ((Doubly*)heur->hcol);
         assert(hcol->drow - d->rmax == heur->num);
         printf("%d ", hcol->dcol);
      }
      printf("\n");
   }
   printf("\n");
}

/*
   use if you want to assert the matrix is stitched together properly
*/
void checkMatrix(Dance *d)
{
   Doubly *hcol, *hrow, *doub;
   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      assert(hcol->left->right == hcol);
      assert(hcol->right->left == hcol);
      for(doub = hcol->down; doub != hcol; doub = doub->down)
         checkDoubly(doub);
   }
   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      assert(hrow->up->down == hrow);
      assert(hrow->down->up == hrow);
   }
}

/* assert doubly is uncovered and stitched properly */
void checkDoubly(Doubly *doub)
{
   assert(doub->left->right == doub);
   assert(doub->right->left == doub);
   assert(doub->up->down == doub);
   assert(doub->down->up == doub);
}
