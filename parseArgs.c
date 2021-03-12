#include "parseArgs.h"
#include "error.h"

void parseArgs(Dance *d, int argc, char *argv[])
{

    if(argc < 1)
        numArgError();

    if(!strcmp(argv[1], "s"))
        d->problem = SUDOKU;
    else if(!strcmp(argv[1], "g"))
        d->problem = SGEN;
    else
        arg1Error();

    switch(d->problem)
    {
        case SUDOKU: parseArgs_Sudoku(d, argc, argv); break;
        case SGEN: parseArgs_SGen(d, argc, argv); break;
    }
}

/*
    a.out s [sudoku file]

    file format:
    [n: number of dimensions]
    next n lines are dimensions
    n must be >= 2

    containerSize: product of all n dimensions, size of a "box"
    sudokuSize: containerSize^n

    next sudokuSize lines are the numbers in the n-dimensional board
*/
void parseArgs_Sudoku(Dance *d, int argc, char *argv[])
{
    int iSudoku, num, test, idim;
    Sudoku *s = malloc(sizeof(Sudoku));
    char *buf = malloc(BUFSIZE*sizeof(char));
    memset(buf, 0, BUFSIZE*sizeof(char));
    d->s = s;

    if(argc != 3)
        numArgError();
    
    s->boardFile = fopen(argv[2], "r+");
    if(!s->boardFile)
        fileError(argv[2]);
    assert(fseek(s->boardFile, 0, SEEK_SET) == 0);

    /* get n */
    fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
    assert(1 == sscanf(buf, "%d", &s->n));
    assert(s->n >= 2);
    s->dim = malloc(s->n*sizeof(int));
    //printf("n: %d\n", s->n);

    s->containerSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
        assert(1 == sscanf(buf, "%d", &s->dim[idim]));
        //printf("dim[%d]: %d\n", idim, s->dim[idim]);
        s->containerSize *= s->dim[idim];
    }
    //printf("containerSize: %d\n", s->containerSize);
    s->sudokuSize = pow(s->containerSize, s->n);
    //printf("sudokuSize: %d\n", s->sudokuSize);
    s->sudoku = calloc(s->sudokuSize, sizeof(int));

    for(iSudoku = 0; iSudoku < s->sudokuSize; iSudoku++)
    {
        fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
        test = sscanf(buf, "%d", &num);
        if(test != 1 || num < 0 || num > s->containerSize)
            invalidSudokuBoard();
        s->sudoku[iSudoku] = num;
    }
    free(buf);
}

/* a.out g [file] [dim]*n */
void parseArgs_SGen(Dance *d, int argc, char *argv[])
{
    Sudoku *s = malloc(sizeof(Sudoku));
    int idim;
    d->s = s;

    if(argc < 4)
        numArgError();

    s->boardFile = fopen(argv[2], "w+");
    if(!s->boardFile)
        fileError(argv[2]);
    assert(fseek(s->boardFile, 0, SEEK_SET) == 0);

    s->n = argc - 3;
    //printf("n: %d\n", s->n);
    fprintf(d->s->boardFile, "%d\n", s->n);

    s->dim = malloc(s->n*sizeof(int));
    s->containerSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        assert(1 == sscanf(argv[idim+3], "%d", &s->dim[idim]));
        //printf("dim[%d]: %d\n", idim, s->dim[idim]);
        fprintf(d->s->boardFile, "%d\n", s->dim[idim]);
        s->containerSize *= s->dim[idim];
    }
    //printf("containerSize: %d\n", s->containerSize);
    s->sudokuSize = pow(s->containerSize, s->n);
    //printf("sudokuSize: %d\n", s->sudokuSize);
    s->sudoku = calloc(s->sudokuSize, sizeof(int));
}