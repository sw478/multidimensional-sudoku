#include "generate.h"
#include "hide.h"
#include "dance.h"
#include "auxil.h"
#include "hrowCover.h"

int generate(Dance *d)
{
    int *hitList, res = NOT_FOUND;
    Hide *h;

    if(d->hideRoot->num >= d->s->maxNumClues)
        return NOT_FOUND;
    
    hitList = calloc(d->s->sudokuSize - d->hideRoot->num, sizeof(int));
    assert(hitList != NULL);
    
    for(h = nextHideRand(d, &hitList); h != d->hideRoot; h = nextHideRand(d, &hitList))
    {
        fillSingleCell(d, h);

        d->numSols = 0;
        d->numCalls = 0;
        coverRowHeaders(d);
        algorithmX_Gen_NumSol(d);
        uncoverRowHeaders(d);
        //printf("number of calls: %lu\n", d->numCalls);
        d->genNumCalls++;
        //if(d->genNumCalls % CALL_TRACKING_GEN == 0)
          //  printf("-----gen calls: %d\n", d->genNumCalls);

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
    }

    free(hitList);
    return res;
}

/* returns a random iSudoku without hiding anything */
Hide *nextHideRand(Dance *d, int **hitList)
{
    Hide *h;
    int i, j, randInt, listSize = d->s->sudokuSize - d->hideRoot->num;
    if(listSize == 0)
        return d->hideRoot;

    randInt = rand() % listSize;
    for(h = d->hideRoot->next, i = 0; i < randInt; h = h->next, i++);
    for(j = i; (*hitList)[j] == 1; h = h->next)
    {
        if(h == d->hideRoot)
            continue;
        j = (j + 1) % listSize;
    }

    (*hitList)[j] = 1;
    return h;
}

/*
    if n == 1, maxNumClues will be ignored and be
    replaced with dim[0] - 1
*/
void setMaxNumClues(Sudoku *s, int maxNumClues)
{
    s->maxNumClues = s->n == 1 ? s->dim[0]-1 : maxNumClues;
}

/* translates filled status of cells to d->s */
void saveGeneratedPuzzle(Dance *d)
{
   int iSudoku;

   for(iSudoku = 0; iSudoku < d->s->sudokuSize; iSudoku++)
   {
      if(d->hideList[iSudoku]->filled == 0)
         d->s->sudoku[iSudoku] = 0;
   }
}

void printToSudokuFile(Dance *d)
{
    int iSudoku, sudokuSize = d->s->sudokuSize, *sudoku = d->s->sudoku;

    for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
        fprintf(d->s->boardFile, "%d\n", sudoku[iSudoku]);
}