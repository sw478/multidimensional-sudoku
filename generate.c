#include "generate.h"
#include "hide.h"
#include "dance.h"
#include "auxil.h"
#include "hrowCover.h"

/**
 * Recursive backtracking algorithm
 * 
 * On each run, "hides" a cell, then solves the altered
 * sudoku and checks if there is still one solution
 * 
 * Keeps going until s->numClues is reached
 */
int generate(Dance *d)
{
    Hide *h;
    int res = NOT_FOUND, listSize;
    int *hitList, sudokuSize = d->s->sudokuSize;

    if(d->hideRoot->num > d->s->numClues)
        return NOT_FOUND;
    
    d->genNumCalls++;
    if(d->genNumCalls >= sudokuSize * THRESHOLD_GEN_FACTOR)
        return NOT_FOUND;
    
    listSize = sudokuSize - d->hideRoot->num;
    //hitList = shuffledHide(d, listSize);
    hitList = calloc(listSize, sizeof(int));
    
    for(h = nextHideRand(d, &hitList); h != d->hideRoot; h = nextHideRand(d, &hitList))
    {
        hideSingleCell(d, h);
        //printSudokuBoard_Gen(d);

        d->numSols = 0;
        d->numCalls = 0;
        coverRowHeaders(d);
        algorithmX_Gen_NumSol(d);
        uncoverRowHeaders(d);
        //printf("number of calls: %lu\n", d->numCalls);
        //if(d->genNumCalls % CALL_TRACKING_GEN == 0)
          //  printf("-----gen calls: %d\n", d->genNumCalls);

        if(d->numSols == 1 && d->hideRoot->num == d->s->numClues)
        {
            saveGeneratedPuzzle(d);
            free(hitList);
            return FOUND;
        }
        if(d->genNumCalls >= sudokuSize * THRESHOLD_GEN_FACTOR)
        {
            unhideSingleCell(d, h);
            free(hitList);
            return NOT_FOUND;
        }
        res = generate(d);

        unhideSingleCell(d, h);
        if(res == FOUND)
            break;
    }

    free(hitList);
    return res;
}

/**
 * Returns a random iSudoku without hiding anything
 */
Hide *nextHideRand(Dance *d, int **hitList)
{
    Hide *h;
    int i, j, randInt, listSize = d->s->sudokuSize - d->hideRoot->num;
    if(listSize == 0)
        return d->hideRoot;

    randInt = rand() % listSize;
    for(h = d->hideRoot->next, i = 0; i < randInt; h = h->next, i++);
    for(j = (i + 1) % listSize; (*hitList)[j] == 1; h = h->next)
    {
        if(h == d->hideRoot)
            continue;
        if(j == i)
        {
            if((*hitList)[j] != 1)
                break;
            return d->hideRoot;
        }
        j = (j + 1) % listSize;
    }

    (*hitList)[j] = 1;
    return h;
}

Hide *nextHideRand2(Dance *d, Hide **hitList, int *irand)
{
    if(*irand == 0)
        return d->hideRoot;

    (*irand)--;

    return hitList[*irand];
}

Hide **shuffledHide(Dance *d, int listSize)
{
    int i, j, irand;
    Hide *temp, **hitList = malloc(listSize*sizeof(Hide*));
    assert(hitList != NULL);

    for(i = 0, j = 0; i < listSize; i++)
    {
        for(; d->hideList[j]->filled; j++);
        hitList[i] = d->hideList[j];
        assert(hitList[i]->filled == 0);
    }

    for(i = listSize - 1; i > 0; i--)
    {
        irand = rand() % (i + 1);
        temp = hitList[irand];
        hitList[irand] = hitList[i];
        hitList[i] = temp;
    }

    return hitList;
}

/**
 * If n == 1, maxNumClues will be ignored and be
 * replaced with dim[0] - 1
*/
void setNumClues(Sudoku *s, int maxNumClues)
{
    s->numClues = s->n == 1 ? s->dim[0]-1 : maxNumClues;
}

/**
 * Translates filled status of cells to sudoku struct
 */
void saveGeneratedPuzzle(Dance *d)
{
   int iSudoku;

   for(iSudoku = 0; iSudoku < d->s->sudokuSize; iSudoku++)
   {
      if(d->hideList[iSudoku]->filled == 0)
         d->s->sudoku[iSudoku] = 0;
   }
}