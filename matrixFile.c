#include "matrixFile.h"

/* call after sudoku board is initialized */
void setMatrixDimensions_Sudoku(Dance *d)
{
   int sudokuSize = d->s->sudokuSize, cs = d->s->containerSize;

   d->rmax = cs * sudokuSize;
   d->cmax = sudokuSize * (2 + d->s->n);
}

/*
   open matrixFile based on dimensions
   create matrixFile if it doesn't exist yet
*/
void findMatrixFile(Dance *d)
{
   char *matrixFile;
   int *sorted_dim = malloc(d->s->n*sizeof(int));
   int idim, n = d->s->n;

   for(idim = 0; idim < n; idim++)
      sorted_dim[idim] = d->s->dim[idim];
   qsort(sorted_dim, n, sizeof(int), compareInt);
   
   matrixFile = getMatrixFileName(d, sorted_dim);

   // check if file doesn't exist
   //if((access(matrixFile, F_OK) != 0))
      createMatrixFile(d, matrixFile, sorted_dim);

   d->matrixFile = fopen(matrixFile, "r+");
   assert(d->matrixFile);

   free(sorted_dim);
   free(matrixFile);
}

int compareInt(const void *a, const void *b)
{
   return *(const int*)a - *(const int*)b;
}

/*
   fileName format:
   dance/dm_d1_d2_d3_..._dn.txt
   with dimensions sorted in increasing order
*/
char *getMatrixFileName(Dance *d, int *sorted_dim)
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));
   char *buf = malloc(BUFSIZE*sizeof(char));
   int idim;
   
   sprintf(matrixFile, "dance/dm");

   for(idim = 0; idim < d->s->n; idim++)
   {
      sprintf(buf, "_%d", sorted_dim[idim]);
      strcat(matrixFile, buf);
   } 
   sprintf(buf, ".txt");
   strcat(matrixFile, buf);

   free(buf);
   return matrixFile;
}

/*
   run the python program to create the matrixFile
*/
void createMatrixFile(Dance *d, char *matrixFile, int *sorted_dim)
{
   int idim;
   char *command = malloc(BUFSIZE*sizeof(char));
   char *buf = malloc(BUFSIZE*sizeof(char));

   sprintf(command, "python matrixFileCreator.py");
   sprintf(buf, " %s", matrixFile);
   strcat(command, buf);
   for(idim = 0; idim < d->s->n; idim++)
   {
      sprintf(buf, " %d", sorted_dim[idim]);
      strcat(command, buf);
   }

   system(command);

   free(command);
}