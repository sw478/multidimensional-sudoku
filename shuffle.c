#include "shuffle.h"

/*
 * randomly shuffles the board into another board
 * within the same symmetry group defined by:
 * permutation of sudoku number labeling: (xy)!
 * swapping rows within any row box and cols
 * within any col box: (x!)^y and (y!)^x
 * swapping box rows and box cols: y! and x!
 * reflecting along the vertical, horizontal,
 * and diagonal centerlines (2^4)
 * 90 degree rotations (4)
 *
 * you could shuffle the board with some certain
 * group operations and end up with the same board
 */
void shuffle(Dance *d)
{
   //relabel(d);
   //shuffleRows(d);
   //shuffleCols(d);
   //shuffleRowBoxes(d);
   //shuffleColBoxes(d);
   reflect(d);
   //rotate(d);
}

void relabel(Dance *d)
{
   int xy = d->s->xy;
   int *grid = d->s->grid, igrid, gridSize = xy*xy;
   int *nums;

   nums = shuffledList(xy);
   for(igrid = 0; igrid < gridSize; igrid++)
      grid[igrid] = nums[grid[igrid]-1]+1;
   free(nums);
   printf("finished number relabeling\n");
}

void shuffleRows(Dance *d)
{
   int *grid = d->s->grid;
   int x = d->s->x, y = d->s->y, xy = d->s->xy;
   int i, brow, row, *rows, *new = malloc(xy*xy*sizeof(int));

   for(brow = 0; brow < y; brow++)
   {
      rows = shuffledList(x);
      for(row = 0; row < x; row++)
      {
         for(i = 0; i < xy; i++)
            new[brow*x*xy + row*xy + i] = grid[brow*x*xy + rows[row]*xy + i];
      }
      free(rows);
   }

   free(grid);
   d->s->grid = new;
   printf("finished swapping rows within row boxes\n");
}

void shuffleCols(Dance *d)
{
   int *grid = d->s->grid;
   int x = d->s->x, y = d->s->y, xy = d->s->xy;
   int i, bcol, col, *cols, *new = malloc(xy*xy*sizeof(int));

   for(bcol = 0; bcol < x; bcol++)
   {
      cols = shuffledList(y);
      for(col = 0; col < y; col++)
      {
         for(i = 0; i < xy; i++)
            new[bcol*y + col + i*xy] = grid[bcol*y + cols[col] + i*xy];
      }
      free(cols);
   }

   free(grid);
   d->s->grid = new;
   printf("finished swapping cols within col boxes\n");
}

void shuffleRowBoxes(Dance *d)
{
   int *grid = d->s->grid, *rows;
   int x = d->s->x, y = d->s->y, xy = d->s->xy, gridSize = d->s->gridSize;
   int igrid, isub, brow, *new = malloc(gridSize*sizeof(int));

   rows = shuffledList(y);
   for(igrid = 0; igrid < gridSize; igrid++)
   {
      brow = igrid / (xy*x);
      isub = igrid % (xy*x);
      new[igrid] = grid[rows[brow]*xy*x + isub];
   }
   free(rows);

   free(grid);
   d->s->grid = new;
   printf("finished swapping row boxes\n");
}

void shuffleColBoxes(Dance *d)
{
   int *grid = d->s->grid, *cols;
   int x = d->s->x, y = d->s->y, xy = d->s->xy, gridSize = d->s->gridSize;
   int igrid, isub, bcol, row, *new = malloc(gridSize*sizeof(int));

   cols = shuffledList(y);
   for(igrid = 0; igrid < gridSize; igrid++)
   {
      row = igrid / xy;
      bcol = igrid % xy / x;
      isub = igrid % y;
      new[igrid] = grid[row*xy + cols[bcol]*x + isub];
   }
   free(cols);

   free(grid);
   d->s->grid = new;
   printf("finished swapping col boxes\n");
}

void reflect(Dance *d)
{
   //if(rand() % 2)
      //reflectHorizontal(d);
   //if(rand() % 2)
      //reflectVertical(d);
   //if(rand() % 2)
      reflectDiagonal(d);

   printf("finished relecting\n");
}

void reflectHorizontal(Dance *d)
{
   int *grid = d->s->grid, i, j;
   int xy = d->s->xy;

   for(i = 0; i < xy; i++)
   {
      for(j = 0; j < xy/2; j++)
         swap(&grid[i*xy + j], &grid[i*xy + xy - j - 1]);
   }
}

void reflectVertical(Dance *d)
{
   int *grid = d->s->grid, i, j;
   int xy = d->s->xy;

   for(i = 0; i < xy/2; i++)
   {
      for(j = 0; j < xy; j++)
         swap(&grid[i*xy + j], &grid[(xy - i - 1)*xy + j]);
   }
}

void reflectDiagonal(Dance *d)
{
   int *grid = d->s->grid, row, col;
   int igrid, xy = d->s->xy, gridSize = xy*xy;
   int *new = malloc(gridSize*sizeof(int));

   for(igrid = 0; igrid < gridSize; igrid++)
   {
      row = igrid / xy;
      col = igrid % xy;
      new[row*xy + col] = grid[col*xy + row];
   }

   free(grid);
   d->s->grid = new;
}

void rotate(Dance *d)
{
   int i, irand = rand() % 4;

   for(i = 0; i < irand; i++)
      rotateOnce(d);

   printf("finished rotating\n");
}

void rotateOnce(Dance *d)
{

}

/*
 * uses the Fisher-Yates O(n) algorithm
 * returns a shuffled list of numbers from 0 to len-1
 */
int *shuffledList(int len)
{
   int i, irand, *nums = malloc(len*sizeof(int));

   for(i = 0; i < len; i++)
      nums[i] = i;

   for(i = len-1; i > 0; i--)
   {
      irand = rand() % (i + 1);
      swap(&nums[irand], &nums[i]);
   }

   //for(i = 0; i < len; i++)
   //   printf("%d ", nums[i]);
   //printf("\n");

   return nums;
}

void swap(int *x, int *y)
{
   int temp = *x;
   *x = *y;
   *y = temp;
}
