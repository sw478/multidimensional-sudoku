#include "auxil.h"
#include "error.h"
#define LINE "=================================================\n\n"

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
   Sudoku *s = d->s;
   int drow, num, iSudoku, containerSize = d->s->containerSize;

   for(cur = sol; cur->parent != cur; cur = cur->parent)
   {
      drow = cur->row->drow;
      num = drow % containerSize;
      iSudoku = drow / containerSize;
      s->sudoku[iSudoku] = num + 1;
   }
   printSudoku(s);
}

/*

   n must be >= 2 to print properly

   currently treats dim0 and dim1 as the innermost dimensions
   later should be able to print using any two dimensions

   subGridSize: containerSize^2 (size of grid to be printed)
   superSize: sudokuSize / subGridSize (number of 2D grids)
   iSudoku: index of cell in sudoku
   iSub: outer index of 2D grid
   iStart: index of first cell of this specific 2D grid in sudoku
   iGrid: index of cell in this specific 2D grid
*/
void printSudoku(Sudoku *s)
{
   int containerSize = s->containerSize;
   int subGridSize = pow(containerSize, 2);
   int superSize = s->sudokuSize / subGridSize;
   int row, col, iSub, iStart, iSudoku, iLine, val;
   int dim0 = s->dim[0], dim1 = s->dim[1];

   assert(s->n >= 2);
   printf(LINE);

   for(iSub = 0; iSub < superSize; iSub++)
   {
      iStart = subGridSize * iSub;
      for(row = 0; row < containerSize; row++)
      {
         if(row % dim1 == 0 && row != 0)
         {
            for(iLine = 0; iLine < containerSize; iLine++)
            {
               if(iLine % dim0 == 0 && iLine != 0)
                  printf("  ");
               printf(" --");
            }
            printf("\n");
         }
         for(col = 0; col < s->containerSize; col++)
         {
            iSudoku = iStart + row*containerSize + col;
            val = s->sudoku[iSudoku];
            if(col % dim0 == 0 && col != 0)
               printf(" |");
            if(val != 0)
               printf(" %2d", val);
            else
               printf(" __");
         }
         printf("\n");
      }
      printf("\n\n");
   }
   printf(LINE);
}

/*
   prints numbers based on whether or not
   d->hideList[iSudoku]->filled == 1
*/
void printSudokuBoard_Gen(Dance *d)
{
   Sudoku *s = d->s;
   int containerSize = s->containerSize;
   int subGridSize = pow(containerSize, 2);
   int superSize = s->sudokuSize / subGridSize;
   int row, col, iSub, iStart, iSudoku, iLine, val;
   int dim0 = s->dim[0], dim1 = s->dim[1];

   assert(s->n >= 2);
   printf(LINE);

   for(iSub = 0; iSub < superSize; iSub++)
   {
      iStart = subGridSize * iSub;
      for(row = 0; row < containerSize; row++)
      {
         if(row % dim1 == 0 && row != 0)
         {
            for(iLine = 0; iLine < containerSize; iLine++)
            {
               if(iLine % dim0 == 0 && iLine != 0)
                  printf("  ");
               printf(" --");
            }
            printf("\n");
         }
         for(col = 0; col < s->containerSize; col++)
         {
            iSudoku = iStart + row*containerSize + col;
            val = s->sudoku[iSudoku];
            if(col % dim0 == 0 && col != 0)
               printf(" |");
            if(d->hideList[iSudoku]->filled != 0)
               printf(" %2d", val);
            else
               printf(" __");
         }
         printf("\n");
      }
      printf("\n");
   }
   printf(LINE);
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

/* checks if matrix is stitched together properly */
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