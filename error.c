#include "error.h"

void error_invalidSudokuBoard()
{
    fprintf(stderr, "Error: Input file contains invalid sudoku board\n");
    exit(EXIT_FAILURE);
}

void error_file(char *fileName)
{
    fprintf(stderr, "FileError: ");
    perror(fileName);
    exit(EXIT_FAILURE);
}

void error_arg1()
{
    fprintf(stderr, "Error: Invalid first argument\n");
    exit(EXIT_FAILURE);
}

void error_randomRowGen()
{
    fprintf(stderr, "Error: RANDOMDIZE_ROWS must be set on\n");
    exit(EXIT_FAILURE);
}

void error_numArg()
{
    fprintf(stderr, "Error: Invalid number of arguments\n");
    exit(EXIT_FAILURE);
}

void error_heurNum()
{
    fprintf(stderr, "Error: Heur num does not match hcol drow\n");
    exit(EXIT_FAILURE);
}

void error_doubly(int drow, int dcol)
{
    fprintf(stderr, "Error: Warped doubly: (drow: %d dcol: %d)\n", drow, dcol);
    exit(EXIT_FAILURE);
}
