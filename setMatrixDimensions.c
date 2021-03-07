#include "setMatrixDimensions.h"

void setMatrixDimensions_Sudoku(Dance *d)
{
   d->rmax = d->s->xy*d->s->gridSize;
   d->cmax = 4*d->s->gridSize;
}

void setMatrixDimensions_Sudoku2(Dance *d)
{
   int x, y, xy, xf, yf, xfy, yfx;
   int fact[11] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};

   x = d->s->x;
   y = d->s->y;
   xy = x*y;
   xf = fact[x];
   yf = fact[y];
   xfy = (int)(pow(xf, y));
   yfx = (int)(pow(yf, x));
   d->rmax = xy * (xfy*yfx);
   d->cmax = xy + xy*xy;
   printf("rmax: %d\n", d->rmax);
   printf("cmax: %d\n", d->cmax);
}

void setMatrixDimensions_NQueens(Dance *d)
{
   int n = d->nq, ndiag;

   ndiag = 4 * n - 6;
   d->rmax = n*n;
   d->cmax = 2*n + ndiag;
   printf("rmax: %d\n", d->rmax);
   printf("cmax: %d\n", d->cmax);
}