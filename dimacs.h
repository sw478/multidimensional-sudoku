#ifndef DIMACS_H
#define DIMACS_H
#include "struct.h"

void writeToDimacs(Dance *d);
int cellToVar(int containerSize, int iSudoku, int val);
void varToCell(Sudoku *s, int containerSize, int var);
int getNumClausesMinimal(Dance *d);
int getNumClausesExtended(Dance *d);
void dimacsHeader(Dance *d, FILE *dimacsFile);
void testConvertSat(Dance *d);

/* minimal */
void dimacsMinimal(Dance *d, FILE *dimacsFile);
void dimacsAtLeastOneValuePerCell(Dance *d, FILE *dimacsFile);
void dimacsAtMostOneValuePerSpan(Dance *d, FILE *dimacsFile, int idim);
void dimacsAtMostOneValuePerContainer(Dance *d, FILE *dimacsFile);

/* extended */
void dimacsExtended(Dance *d, FILE *dimacsFile);
void dimacsAtMostOneValuePerCell(Dance *d, FILE *dimacsFile);
void dimacsAtLeastOneValuePerSpan(Dance *d, FILE *dimacsFile, int idim);
void dimacsAtLeastOneValuePerContainer(Dance *d, FILE *dimacsFile);

/* helper functions */
int iSpanToiSudoku(int iSpan, int idim, int n, int containerSize);
int *inverseDim(Dance *d);
int iContainerToiSudoku(int iContainer, int containerSize, int *dim, int *invDim, int n);
int iContainerToiSudoku2(int offset, int iCell, int containerSize, int *dim, int n);

#endif