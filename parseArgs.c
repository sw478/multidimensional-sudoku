#include "parseArgs.h"
#include "error.h"

int parseFirstArg(int argc, char *argv[])
{
    if(argc < 1)
        numArgError();

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

void parseArgs_DLX_Solve(Dance *d, int argc, char *argv[])
{
    int iSudoku, num, test, idim;
    Sudoku *s = malloc(sizeof(Sudoku));
    char *buf = malloc(BUFSIZE*sizeof(char));
    memset(buf, 0, BUFSIZE*sizeof(char));
    d->s = s;

    if(argc != 4)
        numArgError();
    
    s->boardFile = fopen(argv[2], "r+");
    if(!s->boardFile)
        fileError(argv[2]);
    assert(fseek(s->boardFile, 0, SEEK_SET) == 0);

    s->solFile = fopen(argv[3], "w+");
    if(!s->solFile)
        fileError(argv[3]);
    assert(fseek(s->solFile, 0, SEEK_SET) == 0);

    /* get n */
    fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
    assert(1 == sscanf(buf, "%d", &s->n));
    assert(s->n >= 2);
    s->dim = malloc(s->n*sizeof(int));

    s->containerSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
        assert(1 == sscanf(buf, "%d", &s->dim[idim]));
        s->containerSize *= s->dim[idim];
    }
    s->sudokuSize = round(pow(s->containerSize, s->n));
    s->sudoku = calloc(s->sudokuSize, sizeof(int));

    for(iSudoku = 0; iSudoku < s->sudokuSize; iSudoku++)
    {
        if(!fgets(buf, BUFSIZE*sizeof(char), s->boardFile))
        {
            s->sudoku[iSudoku] = 0;
            continue;
        }
        test = sscanf(buf, "%d", &num);
        if(test != 1 || num < 0 || num > s->containerSize)
            invalidSudokuBoard();
        s->sudoku[iSudoku] = num;
    }
    free(buf);
}

void parseArgs_DLX_Gen(Dance *d, int argc, char *argv[])
{
    Sudoku *s = malloc(sizeof(Sudoku));
    int idim;
    d->s = s;

    if(argc < 5)
        numArgError();

    s->boardFile = fopen(argv[2], "w+");
    if(!s->boardFile)
        fileError(argv[2]);
    assert(fseek(s->boardFile, 0, SEEK_SET) == 0);

    s->solFile = fopen(argv[3], "w+");
    if(!s->solFile)
        fileError(argv[3]);
    assert(fseek(s->solFile, 0, SEEK_SET) == 0);

    s->n = argc - 4;

    s->dim = malloc(s->n*sizeof(int));
    s->containerSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        assert(1 == sscanf(argv[idim+4], "%d", &s->dim[idim]));
        s->containerSize *= s->dim[idim];
    }
    s->sudokuSize = round(pow(s->containerSize, s->n));
    s->sudoku = calloc(s->sudokuSize, sizeof(int));
}

void parseArgs_Enumerate(Dance *d, int argc, char *argv[])
{
    int iSudoku, idim;
    Sudoku *s = malloc(sizeof(Sudoku));
    d->s = s;
    
    if(argc < 4)
        numArgError();

    s->solFile = fopen(argv[2], "w+");
    if(!s->solFile)
        fileError(argv[2]);
    assert(fseek(s->solFile, 0, SEEK_SET) == 0);

    s->n = argc - 3;

    s->dim = malloc(s->n*sizeof(int));
    s->containerSize = 1;
    
    for(idim = 0; idim < s->n; idim++)
    {
        assert(1 == sscanf(argv[idim+3], "%d", &s->dim[idim]));
        s->containerSize *= s->dim[idim];
    }
    
    s->sudokuSize = round(pow(s->containerSize, s->n));
    s->sudoku = calloc(s->sudokuSize, sizeof(int));

    for(iSudoku = 0; iSudoku < s->sudokuSize; iSudoku++)
        s->sudoku[iSudoku] = 0;
}