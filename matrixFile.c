#include "matrixFile.h"

/* call after sudoku board is initialized */
void setMatrixDimensions_Sudoku(Dance *d, Sudoku *s)
{
   d->rmax = s->sudokuSize * s->containerSize;
   d->cmax = s->sudokuSize * (2 + s->n);
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
   int iContainer, iContainerSpan, inum, dividend, j, k, mrow, iConstraint;

   d->matrixFile = fopen(matrixFileName, "w+");
   assert(d->matrixFile);

   int *span = malloc(n * sizeof(int));
   int *span2 = malloc((n-1) * sizeof(int));
   int *iSpan = malloc(n * sizeof(int));
   int *containerMult = malloc(n * sizeof(int));
   int *spanMult = malloc(n * sizeof(int));
   
   containerMult[0] = 1;
   spanMult[0] = 1;
   for (int idim = 1; idim < n; idim++)
   {
      int inverseDim = containerSize / dim[idim-1];
      containerMult[idim] = inverseDim * containerMult[idim-1];
      spanMult[idim] = containerSize * spanMult[idim-1];
   }

   nConstraints = n + 2;
   int *mcol = malloc(nConstraints * sizeof(int));
   for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
   {
      iContainer = 0;
      iSpan = malloc(n*sizeof(int));
      dividend = iSudoku;
      for(idim = 0; idim < n; idim++)
      {
         span[idim] = dividend % containerSize;
         dividend /= containerSize;

         iContainerSpan = span[idim] / dim[idim];
         iContainer += iContainerSpan * containerMult[idim];
      }

      for(idim = 0; idim < n; idim++)
      {
         iSpan[idim] = 0;

         for(j = 0, k = 0; j < n; j++)
         {
            if(j == idim)
               continue;
            span2[k] = span[j];
            k++;
         }

         for(k = 0; k < n-1; k++)
            iSpan[idim] += span2[k] * spanMult[k];
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

   free(containerMult);
   free(spanMult);
   free(span);
   free(span2);
   free(iSpan);
   free(mcol);
   fclose(d->matrixFile);
}