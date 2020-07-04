#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

#define BUFSIZE 1000

typedef struct
{
   int *grid, x, y, xy, z, gridSize;
   int elements, steps, fit, visited, mode;
   FILE *in;
} Sudoku;

void printBoard(int *grid, int x, int y);
void readIn(Sudoku *s);
void parseArgs(Sudoku *s, int argc, char *argv[]);

void invalidInput();
void fileError(char *fileName);
void usage();
void error();
void numArgError();

#endif
