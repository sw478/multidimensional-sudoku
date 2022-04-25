#ifndef DIMACS_H
#define DIMACS_H
#include "struct.h"

void writeToDimacs_Solve(ZChaff *z);
void writeToDimacs_Gen(ZChaff *z);
void readInDimacsOutput(ZChaff *z, FILE *f);
void dimacsHeader(ZChaff *z, int numClauses);

/* misc */
long int cellToVar(int containerSize, int iSudoku, int val);
void varToCell(Sudoku *s, int containerSize, long int var);
int getNumClausesMinimal(Sudoku *s);
int getNumClausesExtended(Sudoku *s);
void testConvertSat(Dance *d);

/* partial */
void dimacsPartial(ZChaff *z);
int getNumClausesPartial(Sudoku *s);

/* minimal */
void dimacsMinimal(ZChaff *z);
void dimacsAtLeastOneValuePerCell(ZChaff *z);
void dimacsAtMostOneValuePerSpan(ZChaff *z, int idim);
void dimacsAtMostOneValuePerContainer(ZChaff *z);

/* extended */
void dimacsExtended(ZChaff *z);
void dimacsAtMostOneValuePerCell(ZChaff *z);
void dimacsAtLeastOneValuePerSpan(ZChaff *z, int idim);
void dimacsAtLeastOneValuePerContainer(ZChaff *z);

/* helper functions */
int iSpanToiSudoku(int iSpan, int idim, int n, int containerSize);
int *inverseDim(Sudoku *s);
int iContainerToiSudoku(int iContainer, int containerSize, int *dim, int *invDim, int n);
int iContainerToiSudoku2(int offset, int iCell, int containerSize, int *dim, int n);

#endif