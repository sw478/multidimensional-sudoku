#ifndef CONVERT_H
#define CONVERT_H
#include "struct.h"

int cellToVar(Dance *d, int iSudoku, int val);
void writeToDimacs(Dance *d);
int getNumClausesMinimal(Dance *d);
int getNumClausesExtended(Dance *d);
void dimacsHeader(FILE *dimacsFile, int numVars, int numClauses);

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

void testConvertSat(Dance *d);

#endif