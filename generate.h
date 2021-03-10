#ifndef GENERATE_H
#define GENERATE_H
#include "struct.h"

int generate(Dance *d);
int generate2(Dance *d);
void printToSudokuFile(Dance *d);
Hide *nextHide(Dance *d, int **hitList);

#endif
