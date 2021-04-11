#ifndef CONVERT_H
#define CONVERT_H
#include "struct.h"

int getVar(Dance *d, int coord, int val);
void writeToDimacs(Dance *d);
void dimacsHeader(FILE *dimacsFile, int numVars, int numClauses);
void dimacsAtLeastOneValuePerCell(Dance *d, FILE *dimacsFile);
void dimacsAtMostOneValuePerSpan(Dance *d, FILE *dimacsFile, int idim);
int iSpanToiSudoku(int iSpan, int idim, int n, int containerSize);
void dimacsAtMostOneValuePerContainer(Dance *d, FILE *dimacsFile);
int iContainerToiSudoku(int iContainer, int containerSize, int *dim, int *invDim, int n);
int *inverseDim(Dance *d);
int iContainerToiSudoku2(int offset, int iCell, int containerSize, int *dim, int n);

void testConvertSat(Dance *d);

#endif