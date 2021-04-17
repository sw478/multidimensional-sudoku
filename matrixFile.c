#include "matrixFile.h"

/* call after sudoku board is initialized */
void setMatrixDimensions_Sudoku(Dance *d)
{
   int sudokuSize = d->s->sudokuSize;

   d->rmax = sudokuSize * d->s->containerSize;
   d->cmax = sudokuSize * (2 + d->s->n);
}

/*
   open matrixFile based on dimensions
   create matrixFile if it doesn't exist yet
*/
void findMatrixFile(Dance *d)
{
   char *matrixFileName = getMatrixFileName(d);

   // create if file doesn't exist
   //if((access(matrixFileName, F_OK) != 0))
   {
      initMatrixFile_Sudoku(d, matrixFileName);
   }

   free(matrixFileName);
}

/*
   fileName format:
   dance/dm_d1_d2_d3_..._dn.txt
   with dimensions sorted in increasing order
*/
char *getMatrixFileName(Dance *d)
{
   char *matrixFileName = malloc(BUFSIZE*sizeof(char));
   char *buf = malloc(BUFSIZE*sizeof(char));
   int idim;
   
   sprintf(matrixFileName, "dance/dm");

   for(idim = 0; idim < d->s->n; idim++)
   {
      sprintf(buf, "_%d", d->s->dim[idim]);
      strcat(matrixFileName, buf);
   } 
   sprintf(buf, ".txt");
   strcat(matrixFileName, buf);

   free(buf);
   return matrixFileName;
}

void initMatrixFile_Sudoku(Dance *d, char *matrixFileName)
{
   int sudokuSize = d->s->sudokuSize, containerSize = d->s->containerSize;
   int n = d->s->n, *dim = d->s->dim, nConstraints, idim, iSudoku;
   int iContainer, iContainerSpan, inum, dividend, j, mrow, iConstraint, mult;

   d->matrixFile = fopen(matrixFileName, "w+");
   assert(d->matrixFile);

   nConstraints = n + 2;

   int *span = malloc(n * sizeof(int));
   int *iSpan = malloc(n * sizeof(int));
   int *mcol = malloc(nConstraints * sizeof(int));

   d->matrixFile = fopen(matrixFileName, "w+");
   assert(d->matrixFile);

   for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
   {
      iContainer = 0;
      iSpan = malloc(n*sizeof(int));
      dividend = iSudoku;
      mult = 1;
      for(idim = 0; idim < n; idim++)
      {
         span[idim] = dividend % containerSize;
         dividend /= containerSize;

         iContainerSpan = span[idim] / dim[idim];
         iContainer += iContainerSpan * mult;
         mult = containerSize / (dim[idim] * mult);
      }

      for(idim = 0; idim < n; idim++)
      {
         mult = 1;
         iSpan[idim] = 0;
         for(j = 0; j < n; j++)
         {
            if(j == idim)
               continue;
            iSpan[idim] += span[j] * mult;
            mult *= containerSize;
         }
      }

      for(inum = 0; inum < containerSize; inum++)
      {
         mrow = iSudoku * containerSize + inum;
         mcol[0] = iSudoku;
         mcol[1] = sudokuSize + (iContainer * containerSize) + inum;

         for(idim = 0; idim < n; idim++)
            mcol[idim+2] = (sudokuSize * (2 + idim)) + iSpan[idim] * containerSize + inum;

         for(iConstraint = 0; iConstraint < nConstraints; iConstraint++)
            fprintf(d->matrixFile, "%d %d\n", mrow, mcol[iConstraint]);
      }
   }

   free(span);
   free(iSpan);
   free(mcol);
   fclose(d->matrixFile);
}