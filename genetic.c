#include "sudoku.h"

/*score will never go higher than this*/
#define MAX 500

/* 
 * pretty inefficient ngl
 * returns number of generations
 */
int genetic(Sudoku *s, int size, int seed, double factor)
{
   int mark, numGens;
   Genetic *g = malloc(sizeof(Genetic));

   init(g, s, size, seed, factor);

   while((mark = iterate(g)) == -1);
   memcpy(s, g->gen[mark], sizeof(Sudoku));
   numGens = g->numGens;

   end(g);
   return numGens;
}

void init(Genetic *g, Sudoku *s, int size, int seed, double factor)
{
   int i;

   g->child = malloc(sizeof(Sudoku));
   g->gen = malloc(size*sizeof(Sudoku));
   g->org = malloc(sizeof(Sudoku));
   g->best = malloc(size*sizeof(int));
   memcpy(g->org, s, sizeof(Sudoku));
   g->size = size;
   g->saved = size*factor;
   g->numGens = 0;
   srandom(seed);

   for(i = 0; i < size; i++)
   {
      g->gen[i] = malloc(sizeof(Sudoku));
      g->gen[i]->elements = g->gen[i]->steps = 0;
   }

   for(i = 0; i < size; i++)
   {
      memcpy(g->gen[i], s, sizeof(Sudoku));
      generateRandom(s, g->gen[i]);
      g->gen[i]->fit = score(s, g->gen[i]);
   }
}

void end(Genetic *g)
{
   int i;

   for(i = 0; i < g->size; i++)
      free(g->gen[i]);
   free(g->gen);
   free(g->org);
   free(g->best);
   free(g->child);
   free(g);
}

/*
 * creates new generation from old one by finding the sudoku boards we don't
 * want to keep, and replacing them with boards of the children of created from
 * two parents from the list of the best boards.
 */
int iterate(Genetic *g)
{
   int i, fit, size = g->size;
   memset(g->best, 0, sizeof(int));

   if(g->numGens % 1000 == 0)
      printf("%d ", g->numGens);

   g->numGens++;
   findBest(g);
   for(i = 0; i < size; i++)
   {
      if(g->best[i] != 0)
         continue;
      if(0 == (fit = nextGen(g, i)))
         return i;
   }
   return -1;
}

int nextGen(Genetic *g, int i)
{
   int p1, p2, size = g->size;

   do
      p1 = random() % size;
   while(g->best[p1] != 1);
   do
      p2 = random() % size;
   while(g->best[p2] != 1);

   createChild(g, p1, p2);
   memcpy(g->gen[i], g->child, sizeof(Sudoku));
   g->gen[i]->fit = score(g->org, g->gen[i]);
   return g->gen[i]->fit;
}

/*
 * stores an array of size g->size of 0 and 1s
 * 1: part of the best, 0: not part of the best
 */
void findBest(Genetic *g)
{
   int i, j, min = MAX, imin = 0;

   memset(g->best, 0, g->size*sizeof(int));
   for(i = 0; i < g->saved; i++)
   {
      for(j = 0, min = MAX; j < g->size; j++)
      {
         if(g->gen[j]->fit < min && !g->best[j])
            min = g->gen[(imin = j)]->fit;
      }
      g->best[imin] = 1;
   }
}

void createChild(Genetic *g, int p1, int p2)
{
   int r, c;
   int rand;

   memcpy(g->child, g->org, sizeof(Sudoku));
   for(r = 0; r < 9; r++)
   {
      for(c = 0; c < 9; c++)
      {
         if(g->org->grid[r*9+c] != 0)
            continue;
         rand = random() % 5;
         if(rand < 2)
            g->child->grid[r*9+c] = g->gen[p1]->grid[r*9+c];
         else if(rand < 4)
            g->child->grid[r*9+c] = g->gen[p2]->grid[r*9+c];
         else
            g->child->grid[r*9+c] = (int)random() % 9 + 1;
         g->child->elements++;
      }
   }
}

void generateRandom(Sudoku *org, Sudoku *rand)
{
   int r, c;

   for(r = 0; r < 9; r++)
   {
      for(c = 0; c < 9; c++)
      {
         if(org->grid[r*9+c] != 0)
            continue;
         rand->grid[r*9+c] = (int)random() % 9 + 1;
         rand->elements++;
      }
   }
}

int score(Sudoku *org, Sudoku *fit)
{
   int r, c, score = 0;

   for(r = 0; r < 9; r++)
   {
      for(c = 0; c < 9; c++)
      {
         if(org->grid[r*9+c] != 0)
            continue;
         score += checkRow(fit, r, c, fit->grid[r*9+c])
            + checkCol(fit, r, c, fit->grid[r*9+c])
            + checkBox(fit, r, c, fit->grid[r*9+c]);
      }
   }
   return score;
}
