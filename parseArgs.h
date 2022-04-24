#ifndef PARSE_H
#define PARSE_H
#include "struct.h"

int parseFirstArg(int argc, char *argv[]);
void parseArgs_DLX_Solve(Dance *d, int argc, char *argv[]);
void parseArgs_DLX_Gen(Dance *d, int argc, char *argv[]);
void parseArgs_Enumerate(Dance *d, int argc, char *argv[]);

#endif