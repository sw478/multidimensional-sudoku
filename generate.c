#include "generate.h"
#include "hide.h"
#include "dance.h"
#include "auxil.h"
#include "hrowCover.h"

int generate(Dance *d)
{
    int *hitList, res = NOT_FOUND;
    Hide *h;

    if(d->hideRoot->num >= d->numClues)
        return NOT_FOUND;
    
    hitList = calloc(d->s->sudokuSize - d->hideRoot->num, sizeof(int));

    h = nextHide(d, &hitList);
    while(h != d->hideRoot)
    {
        fillSingleCell(d, h);

        d->numSols = 0;
        coverRowHeaders(d);
        algorithmX_SGen2(d);
        uncoverRowHeaders(d);
        if(d->numSols == 1)
        {
            saveGeneratedPuzzle(d);
            res = FOUND;
        }
        else
            res = generate(d);
        
        unfillSingleCell(d, h);
        if(res == FOUND)
            break;

        h = nextHide(d, &hitList);
    }

    free(hitList);
    return res;
}

/* returns a random iSudoku without hiding anything */
Hide *nextHide(Dance *d, int **hitList)
{
    Hide *h;
    int i, j, randInt, listSize = d->s->sudokuSize - d->hideRoot->num;
    if(listSize == 0)
        return d->hideRoot;

    /* pick random number in range of number of elements left */
    randInt = rand() % listSize;

    /* go to a random uncovered hide */
    for(h = d->hideRoot->next, i = 0; i < randInt; h = h->next, i++);

    /* continue going through list of uncovered hides until unvisited hide is found */
    for(j = i; (*hitList)[j] == 1; h = h->next)
    {
        if(h == d->hideRoot)
            continue;
        j = (j + 1) % listSize;
    }

    (*hitList)[j] = 1;
    return h;
}

void printToSudokuFile(Dance *d)
{
    int iSudoku, sudokuSize = d->s->sudokuSize, *sudoku = d->s->sudoku;

    for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
        fprintf(d->s->boardFile, "%d\n", sudoku[iSudoku]);
}