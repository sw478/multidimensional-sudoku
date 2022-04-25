#include "parseArgs.h"
#include "error.h"
#include "fileHandling.h"

int parseFirstArg(int argc, char *argv[])
{
    if(argc < 1)
        error_numArg();

    if(!strcmp(argv[1], "s"))
        return DLX_SOLVE;
    else if(!strcmp(argv[1], "g"))
        return DLX_GEN;
    else if(!strcmp(argv[1], "e"))
        return ENUMERATE;
    else if(!strcmp(argv[1], "zs0"))
        return ZCHAFF_SOLVE_0;
    else if(!strcmp(argv[1], "zs1"))
        return ZCHAFF_SOLVE_1;
    else if(!strcmp(argv[1], "zg0"))
        return ZCHAFF_GEN_0;
    else if(!strcmp(argv[1], "zg1"))
        return ZCHAFF_GEN_1;
    else
        return -1;
}

void parseArgs_readDims(Sudoku *s, int argc, char *argv[], int argOffset)
{
    int idim;
    s->n = argc - argOffset;

    s->dim = malloc(s->n*sizeof(int));
    s->containerSize = 1;
    
    for(idim = 0; idim < s->n; idim++)
    {
        assert(1 == sscanf(argv[idim+argOffset], "%d", &s->dim[idim]));
        s->containerSize *= s->dim[idim];
    }
    
    s->sudokuSize = round(pow(s->containerSize, s->n));
    s->sudoku = calloc(s->sudokuSize, sizeof(int));
}

void parseArgs_DLXSolve(DLX *dlx, int argc, char *argv[])
{
    Dance *d = malloc(sizeof(Dance));
    Sudoku *s = malloc(sizeof(Sudoku));
    char *buf = malloc(BUFSIZE*sizeof(char));
    memset(buf, 0, BUFSIZE*sizeof(char));
    dlx->d = d;
    dlx->s = s;
    d->s = s;

    if(argc != 4)
        error_numArg();
    
    dlx->sudokuFile = fopen(argv[2], "r+");
    if(!dlx->sudokuFile)
        error_file(argv[2]);
    assert(fseek(dlx->sudokuFile, 0, SEEK_SET) == 0);

    dlx->solutionFile = fopen(argv[3], "w+");
    if(!dlx->solutionFile)
        error_file(argv[3]);
    assert(fseek(dlx->solutionFile, 0, SEEK_SET) == 0);

    readInSudokuFile(s, dlx->sudokuFile);
}

void parseArgs_DLXGen(DLX *dlx, int argc, char *argv[])
{
    const int argOffset = 4;
    Dance *d = malloc(sizeof(Dance));
    Sudoku *s = malloc(sizeof(Sudoku));
    dlx->d = d;
    dlx->s = s;
    d->s = s;

    if(argc < argOffset + 1)
        error_numArg();

    dlx->sudokuFile = fopen(argv[2], "w+");
    if(!dlx->sudokuFile)
        error_file(argv[2]);
    assert(fseek(dlx->sudokuFile, 0, SEEK_SET) == 0);

    dlx->solutionFile = fopen(argv[3], "w+");
    if(!dlx->solutionFile)
        error_file(argv[3]);
    assert(fseek(dlx->solutionFile, 0, SEEK_SET) == 0);

    parseArgs_readDims(s, argc, argv, argOffset);
}

void parseArgs_Enumerate(Enum *e, int argc, char *argv[])
{
    const int argOffset = 3;
    Sudoku *s = malloc(sizeof(Sudoku));
    Dance *d = malloc(sizeof(Dance));
    e->s = s;
    e->d = d;
    e->d->s = s;
    
    if(argc < argOffset + 1)
        error_numArg();

    e->enumerateFile = fopen(argv[2], "w+");
    if(!e->enumerateFile)
        error_file(argv[2]);
    assert(fseek(e->enumerateFile, 0, SEEK_SET) == 0);

    parseArgs_readDims(s, argc, argv, argOffset);
}

void parseArgs_ZChaffSolve0(ZChaff *z, int argc, char *argv[])
{
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc != 4)
        error_numArg();

    z->dimacsInputFile = fopen(argv[2], "w+");
    if(!z->dimacsInputFile)
        error_file(argv[2]);

    z->sudokuFile = fopen(argv[3], "r+");
    if(!z->sudokuFile)
        error_file(argv[3]);

    readInSudokuFile(s, z->sudokuFile);
}

void parseArgs_ZChaffSolve1(ZChaff *z, int argc, char *argv[])
{
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc != 5)
        error_numArg();

    z->dimacsOutputFile = fopen(argv[2], "r+");
    if(!z->dimacsOutputFile)
        error_file(argv[2]);

    z->sudokuFile = fopen(argv[3], "r+");
    if(!z->sudokuFile)
        error_file(argv[3]);

    z->solutionFile = fopen(argv[4], "w+");
    if(!z->solutionFile)
        error_file(argv[4]);
    assert(fseek(z->solutionFile, 0, SEEK_SET) == 0);
    
    readInSudokuFile(s, z->sudokuFile);
}

void parseArgs_ZChaffGen0(ZChaff *z, int argc, char *argv[])
{
    const int argOffset = 3;
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc < argOffset + 1)
        error_numArg();

    z->dimacsInputFile = fopen(argv[2], "w+");
    if(!z->dimacsInputFile)
        error_file(argv[2]);

    parseArgs_readDims(s, argc, argv, argOffset);
}

void parseArgs_ZChaffGen1(ZChaff *z, int argc, char *argv[])
{
    const int argOffset = 5;
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc < argOffset + 1)
        error_numArg();

    z->dimacsOutputFile = fopen(argv[2], "r+");
    if(!z->dimacsOutputFile)
        error_file(argv[2]);

    z->sudokuFile = fopen(argv[3], "w+");
    if(!z->sudokuFile)
        error_file(argv[3]);
    assert(fseek(z->sudokuFile, 0, SEEK_SET) == 0);

    z->solutionFile = fopen(argv[4], "w+");
    if(!z->solutionFile)
        error_file(argv[4]);
    assert(fseek(z->solutionFile, 0, SEEK_SET) == 0);
    
    parseArgs_readDims(s, argc, argv, argOffset);
}