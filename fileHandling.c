#include "fileHandling.h"
#include "error.h"

void readInSudokuFile(Sudoku *s, FILE *f)
{
    int iSudoku, num, test, idim;
    char *buf = malloc(BUFSIZE*sizeof(char));
    memset(buf, 0, BUFSIZE*sizeof(char));

    fgets(buf, BUFSIZE*sizeof(char), f);
    assert(1 == sscanf(buf, "%d", &s->n));
    assert(s->n >= 2);
    s->dim = malloc(s->n*sizeof(int));

    s->containerSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        fgets(buf, BUFSIZE*sizeof(char), f);
        assert(1 == sscanf(buf, "%d", &s->dim[idim]));
        s->containerSize *= s->dim[idim];
    }
    s->sudokuSize = round(pow(s->containerSize, s->n));
    s->sudoku = calloc(s->sudokuSize, sizeof(int));

    for(iSudoku = 0; iSudoku < s->sudokuSize; iSudoku++)
    {
        if(!fgets(buf, BUFSIZE*sizeof(char), f))
        {
            s->sudoku[iSudoku] = 0;
            continue;
        }
        test = sscanf(buf, "%d", &num);
        if(test != 1 || num < 0 || num > s->containerSize)
            invalidSudokuBoard();
        s->sudoku[iSudoku] = num;
    }
    free(buf);
}

void writeToSudokuFile(Dance *d, FILE *f)
{
    int iSudoku, idim, sudokuSize = d->s->sudokuSize, *sudoku = d->s->sudoku;

    fprintf(f, "%d\n", d->s->n);
    for(idim = 0; idim < d->s->n; idim++)
        fprintf(f, "%d\n", d->s->dim[idim]);

    for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
        fprintf(f, "%d\n", sudoku[iSudoku]);
}

void writeToEnumerateFile(Dance *d, FILE *f)
{
   SolTree *cur;
   int iSol, idim, value, mrow, containerSize = d->s->containerSize;
   
   fprintf(f, "%d\n", d->numSols);
   fprintf(f, "%d\n", d->s->n);
   for(idim = 0; idim < d->s->n; idim++)
      fprintf(f, "%d ", d->s->dim[idim]);

   for(iSol = 0; iSol < d->numSols; iSol++)
   {
      fprintf(f, "\n");

      for(cur = d->sols[iSol]; cur->parent != cur; cur = cur->parent)
      {
         mrow = cur->row->drow;
         value = mrow % containerSize + 1;

         fprintf(f, "%d ", value);
      }
   }
}