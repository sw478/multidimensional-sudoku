#include "auxil.h"
#include "error.h"
#define EQUALS_LINE "=================================================\n"
#define LINE "---------------------------------------\n"

/* this file contains functions useful for debugging and displaying data */

// print by rows, shows hidden rows
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

// print by columns, doesn't show hidden rows
void printMatrix2(Dance *d)
{
   int irow, icol, cmax = d->cmax, rmax = d->rmax, status;
   Doubly *hcol, *doub;

   int **matrix = malloc(cmax*sizeof(int*));
   for(icol = 0; icol < cmax; icol++)
      matrix[icol] = calloc(rmax, sizeof(int));

   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      icol = hcol->dcol;
      for(doub = hcol->down; doub != hcol; doub = doub->down)
      {
         irow = doub->drow;
         matrix[icol][irow] = 1;
      }
   }

   for(irow = 0; irow < rmax; irow++)
   {
      printf("%4d: ", irow);
      for(icol = 0; icol < cmax; icol++)
      {
         status = matrix[icol][irow];
         if(status == 0)
            printf(".");
         else if(status == 1)
            printf("X");
      }
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
}

/* translates single solution from matrix to a sudoku */
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
   currently treats dim0 and dim1 as the innermost dimensions
   later should be able to print using any order of dimensions

   iSudoku: index of cell in sudoku
   iSub: outer index of 2D grid
   iStart: index of first cell of this specific 2D grid in sudoku
*/
void printSudoku(Sudoku *s)
{
   if(s->containerSize == 0)
   {
      printf("not printing, dimensions invalid\n");
      return;
   }

   if(s->n == 1)
   {
      printSudoku_1D(s);
      return;
   }

   printf(EQUALS_LINE);
   printf("\n");

   printSudoku_ND(s, s->n - 1, s->sudokuSize / s->containerSize, 0);

   printf(EQUALS_LINE);
   printf("\n");
}

void printSudoku_ND(Sudoku *s, int idim, int printSize, int iStart)
{
   int dim = s->dim[idim];
   int iSpan, iStartNew;
   int containerSize = s->containerSize;

   if(idim == 1)
   {
      printSudoku_2D(s, iStart);
      return;
   }

   for(iSpan = 0; iSpan < containerSize; iSpan++)
   {
      if(iSpan % dim == 0 && iSpan != 0)
         printf(LINE);
      iStartNew = iStart + iSpan * printSize;
      printf("\nspan[%d]: %d\n\n", idim, iSpan);
      printSudoku_ND(s, idim-1, printSize / containerSize, iStartNew);
   }
}

void printSudoku_2D(Sudoku *s, int iStart)
{
   int containerSize = s->containerSize;
   int row, col, iLine, val, iSudoku;
   int dim0 = s->dim[0], dim1 =s->dim[1];
   
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
      printf("\n");
}

void printSudoku_1D(Sudoku *s)
{
   int iSudoku, val;

   printf(EQUALS_LINE);

   for(iSudoku = 0; iSudoku < s->sudokuSize; iSudoku++)
   {
      val = s->sudoku[iSudoku];
      if(val != 0)
         printf(" %2d", val);
      else
         printf(" __");
   }
   printf("\n");

   printf(EQUALS_LINE);
}

/*
   prints numbers based on whether or not
   d->hideList[iSudoku]->filled == 1
*/
void printSudokuBoard_Gen(Dance *d)
{
   Sudoku *s = d->s;
   int containerSize = s->containerSize;
   int subGridSize = round(pow(containerSize, 2));
   int superSize = s->sudokuSize / subGridSize;
   int row, col, iSub, iStart, iSudoku, iLine, val;
   int dim0 = s->dim[0], dim1 = s->dim[1];

   assert(s->n >= 2);
   printf(EQUALS_LINE);

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
   printf(EQUALS_LINE);
}

void printHeur(Dance *d)
{
   Heur *heur, *head;
   Doubly *hcol;
   int hnum;

   printf("heuristics: \n");

   head = d->heurList[0];
   printf("r: ");
   for(heur = head->next; heur != head; heur = heur->next)
   {
      hcol = ((Doubly*)heur->hcol);
      printf("%d ", hcol->dcol);
   }
   printf("\n");

   for(hnum = 1; hnum < d->maxColElements + 1; hnum++)
   {
      head = d->heurList[hnum];
      if(head->next == head)
         continue;
      printf("%d: ", head->num);
      for(heur = head->next; heur != head; heur = heur->next)
      {
         hcol = ((Doubly*)heur->hcol);
         if(hcol->drow - d->rmax != heur->num)
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

/*
   print to output file all solutions
   one solution per line
   delimeter: space

   0: 1, 2, 3, 4, ...
   1: 1, 2, 4, 3, ...
*/
int outputToFile_Enumerate(Dance *d)
{
   SolTree *cur;
   int iSol, value, mrow, cSize = d->s->containerSize;

   for(iSol = 0; iSol < d->numSols; iSol++)
   {
      for(cur = d->sols[iSol]; cur->parent != cur; cur = cur->parent)
      {
         mrow = cur->row->drow;
         value = mrow % cSize + 1;

         fprintf(d->s->solFile, "%d ", value);

         
      }

      fprintf(d->s->solFile, "\n");
   }

   return 0;
}