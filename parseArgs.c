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
    a.out s [file]

    file format:
    [n: number of dimensions]
    next n lines are dimensions
    n must be >= 2

    boxSize: product of all n dimensions
    sudokuSize: boxSize^n

    next sudokuSize lines are the numbers in the n-dimensional board
*/
void parseArgs_Sudoku(Dance *d, int argc, char *argv[])
{
    int i, c, test, idim;
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
    printf("n: %d\n", s->n);

    s->boxSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
        assert(1 == sscanf(buf, "%d", &s->dim[idim]));
        printf("dim[%d]: %d\n", idim, s->dim[idim]);
        s->boxSize *= s->dim[idim];
    }
    printf("boxSize: %d\n", s->boxSize);
    s->sudokuSize = pow(s->boxSize, s->n);
    printf("sudokuSize: %d\n", s->sudokuSize);
    s->sudoku = calloc(s->sudokuSize, sizeof(int));

    s->subBoxSize = s->dim[0]*s->dim[1];
    s->subGridSize = pow(s->subBoxSize, 2);
    s->superSize = s->sudokuSize / s->subBoxSize;

    for(i = 0; i < s->sudokuSize; i++)
    {
        fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
        test = sscanf(buf, "%d", &c);
        if(test != 1 || c < 0 || c > s->boxSize)
            invalidSudokuBoard();
        s->sudoku[i] = c;
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
    printf("n: %d\n", s->n);
    fprintf(d->s->boardFile, "%d\n", s->n);

    s->dim = malloc(s->n*sizeof(int));
    s->boxSize = 1;
    for(idim = 0; idim < s->n; idim++)
    {
        assert(1 == sscanf(argv[idim+3], "%d", &s->dim[idim]));
        printf("dim[%d]: %d\n", idim, s->dim[idim]);
        fprintf(d->s->boardFile, "%d\n", s->dim[idim]);
        s->boxSize *= s->dim[idim];
    }
    printf("boxSize: %d\n", s->boxSize);
    s->sudokuSize = pow(s->boxSize, s->n);
    printf("sudokuSize: %d\n", s->sudokuSize);
    s->sudoku = calloc(s->sudokuSize, sizeof(int));
}