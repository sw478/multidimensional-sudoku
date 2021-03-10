#include "setMatrixDimensions.h"

/* call after sudoku board is initialized */
void setMatrixDimensions_Sudoku(Dance *d)
{
   d->rmax = d->s->xy*d->s->gridSize;
   d->cmax = 4*d->s->gridSize;
}

/*
   open matrixFile based on dimensions
   create matrixFile if it doesn't exist yet
*/
void findMatrixFile(Dance *d)
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));
   sprintf(matrixFile, "dance/ds1_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);
}

void createMatrixFile(Dance *d)
{

}