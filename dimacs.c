#include "dimacs.h"
#include "error.h"
#include "math.h"

/* convert cell position and cell value to a SAT variable */
int cellToVar(int containerSize, int iSudoku, int val)
{
    return (iSudoku * containerSize) + val + 1;
}

/* saves cell value and position from SAT variable */
void varToCell(Sudoku *s, int containerSize, int var)
{
    assert(var > 0);
    var--;
    s->sudoku[var / containerSize] = var % containerSize;
}

/* writes header and clauses to file from existing sudoku information */
void writeToDimacs(ZChaff *z)
{
    dimacsHeader(z);
    dimacsMinimal(z);
    //dimacsExtended(z);
}

/* writes the minimal portion of sudoku SAT clauses */
void dimacsMinimal(ZChaff *z)
{
    int idim;

    dimacsAtLeastOneValuePerCell(z);
    for(idim = 0; idim < z->s->n; idim++)
        dimacsAtMostOneValuePerSpan(z, idim);
    dimacsAtMostOneValuePerContainer(z);
}

/* writes the extended portion of sudoku SAT clauses */
void dimacsExtended(ZChaff *z)
{
    int idim;

    dimacsAtMostOneValuePerCell(z);
    for(idim = 0; idim < z->s->n; idim++)
        dimacsAtLeastOneValuePerSpan(z, idim);
    dimacsAtLeastOneValuePerContainer(z);
}

/* writes header of dimacs file, including mSudoku information */
void dimacsHeader(ZChaff *z)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;
    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int numVars, numClauses, n = s->n, idim;

    numVars = sudokuSize * containerSize;
    numClauses = getNumClausesMinimal(s);// + getNumClausesExtended(s);

    fprintf(f, "c mSudoku\n");
    fprintf(f, "c n: %d\n", n);
    fprintf(f, "c dim:");
    
    for(idim = 0; idim < n; idim++)
        fprintf(f, " %d", s->dim[idim]);
    fprintf(f, "\nc\n");

    fprintf(f, "p cnf %d %d\n", numVars, numClauses);
}

/* calculates number of minimal clauses */
int getNumClausesMinimal(Sudoku *s)
{
    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int n = s->n;

    return sudokuSize +
        (n + 1) * (sudokuSize * (containerSize*(containerSize-1)/2));
}

/* calculates number of extended clauses */
int getNumClausesExtended(Sudoku *s)
{
    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int n = s->n;

    return sudokuSize * (containerSize*(containerSize-1)/2) + 
        (n + 1) * sudokuSize;
}

void dimacsAtLeastOneValuePerCell(ZChaff *z)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;

    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int iSudoku, val;

    for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
    {
        for(val = 0; val < containerSize; val++)
            fprintf(f, "%d ", cellToVar(containerSize, iSudoku, val));
        fprintf(f, "0\n");
    }
}

void dimacsAtMostOneValuePerSpan(ZChaff *z, int idim)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;

    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int val, n = s->n, partial;

    /*
        a & b are the span indices of cells in the same span
        aSudoku & bSudoku are the sudoku indices of a & b
        aVar & bVar are the SAT vars of a & b
    */
    int a, b, aSudoku, bSudoku, aVar, bVar;

    /* number of span per sudoku, and iterator */
    int iSpan, numSpan = sudokuSize / containerSize;

    /* space between span cells */
    int cellSpace = round(pow(containerSize, idim));

    /* loop through all span of a dimension */
    for(iSpan = 0; iSpan < numSpan; iSpan++)
    {
        /* starting iSudoku index of span */
        partial = iSpanToiSudoku(iSpan, idim, n, containerSize);

        /* loop through all values */
        for(val = 0; val < containerSize; val++)
        {
            /* loop through all pairs of indices within span */
            for(a = 0; a < containerSize-1; a++)
            {
                for(b = a+1; b < containerSize; b++)
                {
                    aSudoku = a * cellSpace + partial;
                    bSudoku = b * cellSpace + partial;
                    aVar = -1*cellToVar(containerSize, aSudoku, val);
                    bVar = -1*cellToVar(containerSize, bSudoku, val);
                    fprintf(f, "%d %d 0\n", aVar, bVar);
                }
            }
        }
    }
}

void dimacsAtMostOneValuePerContainer(ZChaff *z)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;

    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int *dim = s->dim, n = s->n, partial, val;
    int *invDim = inverseDim(s);

    /*
        a & b are the container indices of cells in the same container
        aSudoku & bSudoku are the sudoku indices of a & b
        aVar & bVar are the SAT vars of a & b
    */
    int a, b, aSudoku, bSudoku, aVar, bVar;

    /* number of span per sudoku, and iterator */
    int iContainer, numContainers = sudokuSize / containerSize;

    /* loop through all span of a dimension */
    for(iContainer = 0; iContainer < numContainers; iContainer++)
    {
        partial = iContainerToiSudoku(iContainer, containerSize, dim, invDim, n);

        /* loop through all values */
        for(val = 0; val < containerSize; val++)
        {
            /* loop through all pairs of indices within span */
            for(a = 0; a < containerSize-1; a++)
            {
                for(b = a+1; b < containerSize; b++)
                {
                    aSudoku = iContainerToiSudoku2(partial, a, containerSize, dim, n);
                    bSudoku = iContainerToiSudoku2(partial, b, containerSize, dim, n);
                    aVar = -1*cellToVar(containerSize, aSudoku, val);
                    bVar = -1*cellToVar(containerSize, bSudoku, val);
                    fprintf(f, "%d %d 0\n", aVar, bVar);
                }
            }
        }
    }

    free(invDim);
}

void dimacsAtMostOneValuePerCell(ZChaff *z)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;
    
    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int iSudoku, a, b, aVar, bVar;

    for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
    {
        for(a = 0; a < containerSize-1; a++)
        {
            for(b = a+1; b < containerSize; b++)
            {
                aVar = -1*cellToVar(containerSize, iSudoku, a);
                bVar = -1*cellToVar(containerSize, iSudoku, b);
                fprintf(f, "%d %d 0\n", aVar, bVar);
            }
        }
    }
}

void dimacsAtLeastOneValuePerSpan(ZChaff *z, int idim)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;
    
    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int val, n = s->n, partial, iSudoku, iCell;

    /* number of span per sudoku, and iterator */
    int iSpan, numSpan = sudokuSize / containerSize;

    /* space between span cells */
    int cellSpace = round(pow(containerSize, idim));

    /* loop through all span of a dimension */
    for(iSpan = 0; iSpan < numSpan; iSpan++)
    {
        /* starting iSudoku index of span */
        partial = iSpanToiSudoku(iSpan, idim, n, containerSize);

        for(val = 0; val < containerSize; val++)
        {
            for(iCell = 0; iCell < containerSize; iCell++)
            {
                iSudoku = iCell * cellSpace + partial;
                fprintf(f, "%d ", cellToVar(containerSize, iSudoku, val));
            }
            fprintf(f, "0\n");
        }
    }
}

void dimacsAtLeastOneValuePerContainer(ZChaff *z)
{
    Sudoku *s = z->s;
    FILE *f = z->dimacsInputFile;
    
    int sudokuSize = s->sudokuSize, containerSize = s->containerSize;
    int *dim = s->dim, n = s->n, partial, val;
    int *invDim = inverseDim(s), iSudoku, iCell;

    /* number of span per sudoku, and iterator */
    int iContainer, numContainers = sudokuSize / containerSize;

    /* loop through all span of a dimension */
    for(iContainer = 0; iContainer < numContainers; iContainer++)
    {
        partial = iContainerToiSudoku(iContainer, containerSize, dim, invDim, n);

        /* loop through all values */
        for(val = 0; val < containerSize; val++)
        {
            for(iCell = 0; iCell < containerSize; iCell++)
            {
                iSudoku = iContainerToiSudoku2(partial, iCell, containerSize, dim, n);
                fprintf(f, "%d ", cellToVar(containerSize, iSudoku, val));
            }
            fprintf(f, "0\n");
        }
    }

    free(invDim);
}

/* gets the index of the first cell of the span with index iSpan of this idim */
int iSpanToiSudoku(int iSpan, int idim, int n, int containerSize)
{
    int i, mult = 1, partial, iSudoku = 0;
    
    for(i = 0; i < n; i++)
    {
        if(i != idim)
        {
            partial = iSpan % containerSize;
            iSpan /= containerSize;
            iSudoku += partial * mult;
        }
        mult *= containerSize;
    }

    return iSudoku;
}

/*
    returns a list of the "inverse" of the dimensions,
    which is just containerSize / dim

    used for calculating container indices
*/
int *inverseDim(Sudoku *s)
{
    int n = s->n, *dim = s->dim, containerSize = s->containerSize;
    int *invDim = malloc(n*sizeof(int));

    for(int i = 0; i < n; i++)
        invDim[i] = containerSize / dim[i];

    return invDim;
}

/* gets the index of the first cell of the container with index iContainer */
int iContainerToiSudoku(int iContainer, int containerSize, int *dim, int *invDim, int n)
{
    int i, mult = 1, partial, iSudoku = 0;
    
    for(i = 0; i < n; i++)
    {
        partial = (iContainer % invDim[i]) * dim[i];
        iContainer /= invDim[i];
        iSudoku += partial * mult;
        mult *= containerSize;
    }

    return iSudoku;
}

/*
    calculates the iSudoku of a specific cell in a container given the position
    of the first cell in the container (offset) and the index of the cell in the
    container (iCell)
*/
int iContainerToiSudoku2(int offset, int iCell, int containerSize, int *dim, int n)
{
    int i, mult = 1, partial, iSudoku = offset;
    
    for(i = 0; i < n; i++)
    {
        partial = iCell % dim[i];
        iCell /= dim[i];
        iSudoku += partial * mult;
        mult *= containerSize;
    }

    return iSudoku;
}

/* some tests to verify correctness of the calculated mSudoku indices */
void testConvertSat(Dance *d)
{
    int containerSize = d->s->containerSize, sudokuSize = d->s->sudokuSize;
    int n = d->s->n, *dim = d->s->dim, *invDim, offset;
    int iSudoku, iCell, idim, iSpan, iContainer;
    int invContainerSize = sudokuSize / containerSize;

    /* verifies the first index of all spans */
    for(idim = 0; idim < n; idim++)
    {
        printf("\tidim: %d\n", idim);
        for(iSpan = 0; iSpan < sudokuSize / containerSize; iSpan++)
        {
            iSudoku = iSpanToiSudoku(iSpan, idim, n, containerSize);
            printf("%d ", iSudoku);
        }
        printf("\n");
    }

    /* verifies the calculated inverse dimensions */
    invDim = inverseDim(d->s);
    printf("\tinv dim\n");
    for(idim = 0; idim < n; idim++)
        printf("%d ", invDim[idim]);
    printf("\n");

    /* verifies the first index of all containers */
    printf("\tcontainers\n");
    for(iContainer = 0; iContainer < invContainerSize; iContainer++)
    {
        iSudoku = iContainerToiSudoku(iContainer, containerSize, dim, invDim, n);
        printf("%d ", iSudoku);
    }
    printf("\n");

    /* verifies the correct iSudoku for cells in a single container from the given offset */
    offset = 0;
    for(iCell = 0; iCell < containerSize; iCell++)
    {
        iSudoku = iContainerToiSudoku2(offset, iCell, containerSize, dim, n);
        printf("%d ", iSudoku);
    }

    free(invDim);
}