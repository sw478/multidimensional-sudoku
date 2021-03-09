#include "error.h"

void invalidSudokuBoard()
{
   fprintf(stderr, "Error: Input file contains invalid sudoku board\n");
   exit(EXIT_FAILURE);
}

void fileError(char *fileName)
{
   fprintf(stderr, "FileError: ");
   perror(fileName);
   exit(EXIT_FAILURE);
}

void arg1Error()
{
   fprintf(stderr, "Error: Invalid first argument\n");
   exit(EXIT_FAILURE);
}

void randomRowGenError()
{
   fprintf(stderr, "Error: RANDOMDIZE_ROWS must be set on\n");
   exit(EXIT_FAILURE);
}

void numArgError()
{
   fprintf(stderr, "Error: Invalid number of arguments\n");
   exit(EXIT_FAILURE);
}

void heurNumError()
{
   fprintf(stderr, "Error: Heur num does not match hcol drow\n");
   exit(EXIT_FAILURE);
}

void invalidN_NQueens()
{
   fprintf(stderr, "Error: Invalid value for n\n");
   exit(EXIT_FAILURE);
}

void checkDoublyError(int drow, int dcol)
{
   fprintf(stderr, "Error: Warped doubly: (drow: %d dcol: %d)\n", drow, dcol);
   exit(EXIT_FAILURE);
}

void error()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}
