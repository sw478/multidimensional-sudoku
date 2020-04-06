#include "sudoku.h"

/*
 * reads in text file containing numbers 0-9, 0 denoting an unknown
 * space
 *
 * If a textfile contains less characters than required, a message will
 * display. Any extra characters will be ignored
 *
 * this is the first iteration so only 9x9 will be considered
 */
int main(int argc, char *argv[])
{
   Sudoku *org = malloc(sizeof(Sudoku)), *s = malloc(sizeof(Sudoku));
   FILE *in;

   checkArgs(org, argc, argv, &in);
   readIn(org, in);
   org->steps = 0;

   printBoard(org);

   memcpy(s, org, sizeof(Sudoku));
   dfs(s);
   printBoard(s);

   memcpy(s, org, sizeof(Sudoku));
   /*genetic(s, 100, 0, 0.1);*/
   /*printBoard(s);*/

   initDance(s);
   printBoard(s);

   fclose(in);
   free(s);
   free(org);
   return 0;
}

int place(Sudoku *s, int row, int col, int value)
{
   if(checkBoard(s, row, col, value))
      return 1;
   s->grid[row*s->n2+col] = value;
   s->elements++;
   return 0;
}

int checkBoard(Sudoku *s, int row, int col, int value)
{
   if(s->grid[row*s->n2+col] != 0)
      return 1;
   if(value < 1 || value > s->n2)
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

   for(i = 0; i < s->n2; i++)
   {
      if(s->grid[row*s->n2+i] == value && i != col)
         return 1;
   }
   return 0;
}

int checkCol(Sudoku *s, int row, int col, int value)
{
   int i;

   for(i = 0; i < s->n2; i++)
   {
      if(s->grid[i*s->n2+col] == value && i != row)
         return 1;
   }
   return 0;
}

int checkBox(Sudoku *s, int row, int col, int value)
{
   int i, j, r = row - row % s->n, c = col - col % s->n;

   for(i = r; i < r + s->n; i++)
   {
      for(j = c; j < c + s->n; j++)
      {
         if(s->grid[i*s->n2+j] == value && i != row && j != col)
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
   for(row = 0; row < s->n2; row++)
   {
      if(row != 0 && row % s->n == 0)
         printf("------|-------|------\n");
      for(col = 0; col < s->n2; col++)
      {
         if(col != 0)
            printf(" ");
         if(col != 0 && col % s->n == 0)
            printf("| ");
         if(s->grid[row*s->n2+col] != 0)
            printf("%d", s->grid[row*s->n2+col]);
         else
            printf("_");
      }
      printf("\n");
   }
   printf("elements: %d\n", s->elements);
   printf("steps: %d\n", s->steps);
}

void readIn(Sudoku *s, FILE *in)
{
   int row, col;
   int c, elements = 0;

   for(row = 0; row < s->n2; row++)
   {
      for(col = 0; col < s->n2; col++)
      {
         c = (int)fgetc(in) - 48;
         if((int)c < 0 || (int)c > s->n2)
            invalidInput();
         s->grid[row*s->n2+col] = (int)c;
         if((int)c != 0)
            elements++;
      }
   }
   s->elements = elements;
}

void checkArgs(Sudoku *s, int argc, char *argv[], FILE **in)
{
   if(argc != 2)
      usage();
   *in = fopen(argv[1], "r");
   if(!*in)
      fileError(argv[1]);
   s->n = (int)sqrt(SIZE);
   s->n2 = SIZE;
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
