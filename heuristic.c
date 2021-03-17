#include "heuristic.h"
#include "error.h"

/* using heur */
Doubly *heuristic(Dance *d)
{
   Heur *heurHeader;
   int hnum, listSize = d->maxColElements + 1;

   for(hnum = 1; hnum < listSize; hnum++)
   {
      heurHeader = d->heurList[hnum];
      if(heurHeader->next != heurHeader)
         return heurHeader->next->hcol;
   }

   return d->root;
}

/* original setup, O(cmax) time */
Doubly *heuristic2(Dance *d)
{
   Doubly *hcol, *minXs;

   for(hcol = minXs = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      if(hcol->drow < minXs->drow)
         minXs = hcol;
   }

   return minXs;
}

/*
   d->heurRoot acts as a header for hcols with 0 elements underneath

   maxColElements is optional, set to 0 if unknown
   maxColElements is just the maximum amount of elements underneath an hcol possible
*/
void initHeurList(Dance *d, int maxColElements)
{
   Doubly *hcol;
   Heur *heur, *heurHeader;
   int hnum;

   assert(maxColElements > 0 && maxColElements <= d->rmax);

   d->heurList = malloc((maxColElements+1) * sizeof(Heur));
   d->maxColElements = maxColElements;

   for(hnum = 0; hnum < maxColElements+1; hnum++)
      d->heurList[hnum] = initHeur(hnum);
   
   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      hnum = hcol->drow - d->rmax;
      assert(hnum <= maxColElements);
      heur = initHeur(hnum);
      heur->hcol = ((void*)hcol);
      hcol->heur = heur;

      heurHeader = d->heurList[hnum];
      heur->next = heurHeader;
      heur->prev = heurHeader->prev;
      heurHeader->prev->next = heur;
      heurHeader->prev = heur;
   }
}

Heur *initHeur(int num)
{
   Heur *heur = malloc(sizeof(Heur));
   heur->prev = heur->next = heur;
   heur->num = num;

   return heur;
}

void incHeur(Dance *d, Heur *heur, int amount)
{
   Heur *head;

   // remove heur from current heur sublist
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;
   heur->num += amount;
   
   head = d->heurList[heur->num];

   //assert(((Doubly*)heur->hcol)->drow - d->rmax == heur->num);

   // add heur to heur header
   heur->next = head;
   heur->prev = head->prev;
   head->prev->next = heur;
   head->prev = heur;
}

void decHeur(Dance *d, Heur *heur, int amount)
{
   Heur *head;

   // remove heur from current heur sublist
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;
   heur->num -= amount;

   head = d->heurList[heur->num];

   //assert(((Doubly*)heur->hcol)->drow - d->rmax == heur->num);

   // add heur to heur header
   heur->next = head;
   heur->prev = head->prev;
   head->prev->next = heur;
   head->prev = heur;
}

void freeHeur(Dance *d)
{
   int hnum;

   for(hnum = 0; hnum < d->maxColElements+1; hnum++)
      freeHeurHead(d->heurList[hnum]);
   free(d->heurList);
}

void freeHeurHead(Heur *head)
{
   Heur *heur, *temp;
   for(heur = head->next; heur != head;)
   {
      temp = heur;
      heur = heur->next;
      free(temp);
   }
   free(head);
}
