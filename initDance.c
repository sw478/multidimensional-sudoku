#include "initDance.h"
#include "solTree.h"
#include "dance.h"

/*
 * Initializes secondary values and structs in the dance struct
 */
int initDance(Dance *d)
{
   d->numSols = 0;
   d->solCap = STARTING_CAP;
   d->sols = malloc(d->solCap*sizeof(SolTree*));

   d->numCalls = 0;

   d->sec_hcol_index = d->cmax; /* default */

   return 0;
}