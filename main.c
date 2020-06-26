#include "sudoku.h"
#include "dance.h"
#include "setup.h"

/*
 * reads in text file containing positive integers, 0 denoting an unknown
 * space
 *
 * If a textfile contains less characters than required, a message will
 * display. Any extra characters will be ignored
 *
 * currently transitioning from 3x3 boards to other variants, such
 * as 2x2 or 3x4 boards
 *
 */
int main(int argc, char *argv[])
{
   Sudoku *org = malloc(sizeof(Sudoku)), *s = malloc(sizeof(Sudoku));
   FILE *in;

   checkArgs(org, argc, argv, &in);
   readIn(org, in);

   printBoard(org);

   run(org, initDanceSudoku);

   fclose(in);
   free(org->grid);
   free(s);
   free(org);

   return 0;
}

void run(Sudoku *org, int(*solve)(Sudoku *s))
{
   Sudoku *s = malloc(sizeof(Sudoku));

   memcpy(s, org, sizeof(Sudoku));
   s->grid = malloc(org->gridSize*sizeof(int));
   memcpy(s->grid, org->grid, org->gridSize*sizeof(int));
   solve(s);
   printBoard(s);

   free(s->grid);
   free(s);
}

int checkBoard(Sudoku *s, int row, int col, int value)
{
   if(s->grid[row*s->xy + col] != 0)
      return 1;
   if(value < 1 || value > s->xy)
      return 1;
   return checkRow(s, row, col, value) ||
      checkCol(s, row, col, value) || checkBox(s, row, col, value);
}

/*
 * checks the row containing the cell with (row, col) for validity
 * valid if unique element in row
 * returns 1 if invalid, 0 if valid
 */
int checkRow(Sudoku *s, int row, int col, int value)
{
   int i;

   for(i = 0; i < s->xy; i++)
   {
      if(s->grid[row*s->xy + i] == value && i != col)
         return 1;
   }
   return 0;
}

int checkCol(Sudoku *s, int row, int col, int value)
{
   int i;

   for(i = 0; i < s->xy; i++)
   {
      if(s->grid[i*s->xy + col] == value && i != row)
         return 1;
   }
   return 0;
}

int checkBox(Sudoku *s, int row, int col, int value)
{
   int i, j;
   int r = row - row % s->x, c = col - col % s->y;

   for(i = r; i < r + s->y; i++)
   {
      for(j = c; j < c + s->x; j++)
      {
         if(s->grid[i*s->xy + j] == value && i != row && j != col)
            return 1;
      }
   }
   return 0;
}

/*
 * for obvious debugging purposes
 */
void printBoard(Sudoku *s)
{
   int row, col;

   printf("\n");
   for(row = 0; row < s->xy; row++)
   {
      if(row % s->y == 0)
         printf("\n");
      for(col = 0; col < s->xy; col++)
      {
         if(col % s->x == 0)
            printf(" ");
         if(s->grid[row*s->xy + col] != 0)
            printf(" %2d", s->grid[row*s->xy + col]);
         else
            printf(" __");
      }
      printf("\n");
   }
}

void readIn(Sudoku *s, FILE *in)
{
   int x, y, length, gridSize;
   int i, c, test;
   char buf[BUFSIZE];

   fgets(buf, sizeof(buf), in);
   test = sscanf(buf, "%d %d", &s->y, &s->x);
   if(test != 2)
      invalidInput();
   x = s->x;
   y = s->y;
   s->xy = length = x*y;
   s->gridSize = gridSize = length*length;
   s->grid = malloc(gridSize*sizeof(int));

   for(i = 0; i < gridSize; i++)
   {
      fgets(buf, BUFSIZE, in);
      test = sscanf(buf, "%d", &c);
      if(test < 1)
         invalidInput();
      if(c < 0 || c > length)
         invalidInput();
      s->grid[i] = c;
   }
}

void checkArgs(Sudoku *s, int argc, char *argv[], FILE **in)
{
   if(argc != 2)
      usage();
   *in = fopen(argv[1], "r");
   if(!*in)
      fileError(argv[1]);
}

void invalidInput()
{
   fprintf(stderr, "Input file contains invalid game board\n");
   exit(EXIT_FAILURE);
}

void fileError(char *fileName)
{
   fprintf(stderr, "FileError: ");
   perror(fileName);
   exit(EXIT_FAILURE);
}

void usage()
{
   fprintf(stderr, "Usage: ./a.out [sudoku file]\n");
   exit(EXIT_FAILURE);
}

void error()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}
