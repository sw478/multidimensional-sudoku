#ifndef FREE_H
#define FREE_H
#include "struct.h"

void free_DLXSolve(DLX *dlx);
void free_DLXGenFull(DLX *dlx);
void free_DLXGenPartial(DLX *dlx);
void free_ZChaffSolve0(ZChaff *z);
void free_ZChaffSolve1(ZChaff *z);
void free_ZChaffGen0(ZChaff *z);
void free_ZChaffGen1(ZChaff *z);
void free_Enum(Enum *e);
void free_Hide(Dance *d);
void free_Sudoku(Sudoku *s);
void free_Dance(Dance *d);
void free_Matrix(Dance *d);
void free_Column(Doubly *col);

#endif
