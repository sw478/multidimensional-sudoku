#include "auxil.h"
#include "error.h"

#define PRINT_MEM_DOUBLY(doubly) { printf("drow: %3d dcol: %3d mem: %p\n", (doubly)->drow, (doubly)->dcol, ((void*)(doubly))); }

/* this file contains functions useful for debugging and displaying data */

void printMatrix(Dance *d)
{
   int pcol = 0, irow, nrow;
   Doubly *xcol, *hrow;

   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      nrow = hrow->dcol - d->cmax;
      if(nrow == 0)
         continue;
      printf("%4d: ", hrow->drow);
      pcol = 0;
      irow = 0;
      xcol = hrow->right;
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

void printSingleSol_Matrix(Dance *d, SolTree *sol)
{
   SolTree *cur;
   Doubly *xcol, *xrow;
   int pcol, irow, nrow;

   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      xrow = cur->row;
      nrow = xrow->dcol - d->cmax;
      if(nrow == 0)
         continue;
      printf("%4d: ", xrow->drow);
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

void printSolutions_Sudoku(Dance *d)
{
   int i;

   printf("\n%d solutions found\n", d->numSols);
   if(d->numSols != 1)
      return;

   for(i = 0; i < d->numSols; i++)
   {
      printf("\nsol %d: \n", i + 1);
      //printSingleSol_Matrix(d, d->sols[i]); /* prints rows of matrices */
      printSingleSol_Sudoku(d, d->sols[i]); /* prints the solution as a sudoku grid */
   }
   printf("\n");
}

/* TODO */
void printSingleSol_Sudoku(Dance *d, SolTree *sol)
{
   SolTree *cur;
   int drow, num, igrid, xy = d->s->xy, *grid;
   grid = calloc(d->cmax, sizeof(int));

   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      drow = cur->row->drow;
      num = drow % xy;
      igrid = drow / xy;
      grid[igrid] = num + 1;
   }
   printSudoku(d->s);
   free(grid);
}

/*
   2D representation will be printed using the first two inner dimensions
   iSub: index of 2d grid, range is 0 to superSize
*/
void printSudoku(Sudoku *s)
{
   int iSub, superSize = s->superSize;

   for(iSub = 0; iSub < superSize; iSub++)
      printSudoku_2D(s, iSub, s->dim[0], s->dim[1]);
   printf("\n");
}

/*
   currently treats dim0 and dim1 as the innermost dimensions
   later should be able to print using any two dimensions

   subBoxSize: dim0*dim1 (size of box in 2D grid)
   subGridSize: subBoxSize^2 (size of grid to be printed)
   superSize: sudokuSize / subGridSize (number of 2D grids)
   iSudoku: index of cell in sudoku
   iStart: index of first cell of this specific 2D grid in sudoku
   iGrid: index of cell in this specific 2D grid
*/
void printSudoku_2D(Sudoku *s, int iSub, int dim0, int dim1)
{
   int subBoxSize = dim0*dim1, subGridSize = pow(subBoxSize, 2);
   int row, col, iStart = subGridSize * iSub, iGrid, val;

   for(row = 0; row < subBoxSize; row++)
   {
      if(row % dim1 == 0)
         printf("\n");
      for(col = 0; col < subBoxSize; col++)
      {
         iGrid = row*subBoxSize + col;
         val = s->sudoku[iStart + iGrid];
         if(col % dim0 == 0)
            printf(" ");
         if(val != 0)
            printf(" %2d", val);
         else
            printf(" __");
      }
      printf("\n");
   }
   printf("\n");
}

/*
   prints numbers based on whether or not
   d->hideList[igrid]->filled == 1
*/
void printSudokuBoard_Gen(Dance *d)
{
   int x = d->s->x, y = d->s->y;
   int row, col, xy = x*y, igrid;

   for(row = 0; row < xy; row++)
   {
      if(row % y == 0)
         printf("\n");
      for(col = 0; col < xy; col++)
      {
         igrid = row*xy + col;
         if(col % x == 0)
            printf(" ");
         if(d->hideList[igrid]->filled)
            printf(" %2d", d->hideList[igrid]->num);
         else
            printf(" __");
      }
      printf("\n");
   }
   printf("\n");
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
         if(hcol->drow - d->rmax == heur->num)
            heurNumError();
         printf("%d ", hcol->dcol);
      }
      printf("\n");
   }
   printf("\n");
}

void checkMatrix(Dance *d)
{
   Doubly *hcol, *hrow, *doub;
   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      if(!(hcol->left->right == hcol && hcol->right->left == hcol))
         checkDoublyError(hcol->drow, hcol->dcol);
      for(doub = hcol->down; doub != hcol; doub = doub->down)
         checkDoubly(doub);
   }
   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      if(!(hrow->up->down == hrow && hrow->down->up == hrow))
         checkDoublyError(hrow->drow, hrow->dcol);
   }
}

void checkDoubly(Doubly *doub)
{
   if(!(doub->left->right == doub &&
         doub->right->left == doub &&
         doub->up->down == doub &&
         doub->down->up == doub))
      checkDoublyError(doub->drow, doub->dcol);
}

void printMatrixDoublyMemory(Dance *d)
{
   Doubly *hcol, *hrow, *doub;

   PRINT_MEM_DOUBLY(d->root)
   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      PRINT_MEM_DOUBLY(hcol)
      for(doub = hcol->down; doub != hcol; doub = doub->down)
         PRINT_MEM_DOUBLY(doub)
   }
   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
      PRINT_MEM_DOUBLY(hrow)
}

void printBinary(uint64_t num)
{
    int bit, ibit;

    printf(" ");
    for(ibit = 0; ibit < 16; ibit++)
    {
        bit = num % 2;
        num = num / 2;
        printf("%d", bit);
    }
}