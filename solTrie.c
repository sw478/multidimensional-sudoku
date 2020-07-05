#include "solTrie.h"
#include "dance.h"

void addLeaf(Dance *d)
{
   if(d->numSols >= d->solCap)
   {
      d->solCap *= 2;
      d->sols = realloc(d->sols, (d->solCap)*sizeof(SolTrie));
   }
   d->sols[d->numSols] = d->csol;
   d->numSols++;
}

SolTrie* initTrie(void *row)
{
   SolTrie *new = malloc(sizeof(SolTrie));
   new->row = row;
   new->ichild = new->numSols = 0;
   new->cap = STARTING_CAP;
   new->child = malloc(new->cap*sizeof(SolTrie));
   new->parent = new;

   return new;
}

void addChild(SolTrie *sol, SolTrie *new)
{
   if(sol->ichild >= sol->cap)
   {
      sol->cap *= 2;
      sol->child = realloc(sol->child, sol->cap*sizeof(SolTrie));
   }

   sol->child[sol->ichild] = new;
   sol->ichild++;

   new->parent = sol;
}

void incNumSols(SolTrie *sol)
{
   SolTrie *temp;
   for(temp = sol; temp->parent != temp; temp = temp->parent)
      temp->numSols++;
   temp->numSols++;
}

/* 
 * finds the child with the given row, removes and frees the solTrie,
 * and shifts all the solTries behind it forward
 * returns number of solTries found and deleted
 * (0 if not found, 1 if found, 2+ if duplicate was found)
 * if duplicates found, duplicates will also be freed and removed
 */
int deleteChild(SolTrie *sol, void *row)
{
   int i, found = 0;

   for(i = 0; i < sol->ichild; i++)
   {
      if(found > 0)
         sol->child[i - found] = sol->child[i];
      if(sol->child[i]->row == row)
      {
         found++;
         freeSol(sol->child[i]);
      }
   }
   sol->child[i] = NULL;
   sol->ichild -= found;

   return found;
}

int freeSol(SolTrie *sol)
{
   int i, total = 0;

   for(i = 0; i < sol->ichild; i++)
      total += freeSol(sol->child[i]);

   free(sol->child);
   free(sol);

   return total;
}
