#include "heuristic.h"

void initHeurList(Dance *d)
{
   Doubly *hcol;
   Heur *heur, *head, *temp;
   d->heurRoot = initHeur(0);
   d->heurRoot->hcol = ((void*)d->root);

   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      heur = initHeur(hcol->drow - d->rmax);
      heur->hcol = ((void*)hcol);
      hcol->heur = heur;

      head = d->heurRoot->hnext;
      for(; heur->num < head->num; head = head->hnext);
      if(heur->num != head->num)
      {
         temp = initHeur(heur->num);
         temp->hprev = head->prev;
         temp->hnext = head;
         head->hprev->hnext = temp;
         head->hprev = temp;
         head = temp;
      }

      heur->next = head;
      heur->prev = head->prev;
      head->prev->next = heur;
      head->prev = heur;
      heur->head = head;
   }
}

Heur *initHeur(int num)
{
   Heur *heur = malloc(sizeof(Heur));
   heur->head = heur->prev = heur->next = heur;
   heur->hprev = heur->hnext = heur;
   heur->num = num;

   return heur;
}

void incHeur(Heur *heur)
{
   Heur *head = heur->head, *temp;

   heur->num++;
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;

   if(head->next == head && head->num != 0)
   {
      temp = head->hprev;  
      head->hnext->hprev = head->hprev;
      head->hprev->hnext = head->hnext;
      free(head);
      head = temp;
   }

   head = head->hnext;
   if(head->num != heur->num)
   {
      temp = initHeur(heur->num);
      temp->hprev = head->hprev;
      temp->hnext = head;
      head->hprev->hnext = temp;
      head->hprev = temp;
      head = temp;
   }
   heur->next = head->next;
   heur->prev = head;
   head->next->prev = heur;
   head->next = heur;
   heur->head = head;
}

void decHeur(Heur *heur)
{
   Heur *head = heur->head, *temp;

   heur->num--;
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;

   if(head->next == head && head->num != 0)
   {
      temp = head->hnext;
      head->hnext->hprev = head->hprev;
      head->hprev->hnext = head->hnext;
      free(head);
      head = temp;
   }

   head = head->hprev;
   if(head->num != heur->num)
   {
      temp = initHeur(heur->num);
      temp->hnext = head->hnext;
      temp->hprev = head;
      head->hnext->hprev = temp;
      head->hnext = temp;
      head = temp;
   }

   heur->next = head->next;
   heur->prev = head;
   head->next->prev = heur;
   head->next = heur;
   heur->head = head;
}

void freeHeur(Dance *d)
{
   Heur *head, *temp;

   for(head = d->heurRoot->hnext; head != d->heurRoot;)
   {
      freeHeurHead(head);
      temp = head;
      head = head->hnext;
      free(temp);
   }
   freeHeurHead(head);

   free(d->heurRoot);
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
}
