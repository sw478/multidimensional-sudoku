#ifndef CONFIG_H
#define CONFIG_H

/*
    set to 1 to use heur system

    will call decHeur or incHeur each time
    a doubly is covered oor uncovered
*/
#define USE_HEUR 1

#if USE_HEUR == 1
    #define HEUR_HEURISTIC(d) { hcol = heuristic((d));}
    #define HEUR_INIT(d, heur_max) { initHeurList((d), (heur_max)); }
    #define HEUR_DEC(d, dcol, index, heur) { if((dcol) < (index)) { decHeur((d), (heur)); } }
    #define HEUR_INC(d, dcol, index, heur) { if((dcol) < (index)) { incHeur((d), (heur)); } }
    #define HEUR_FREE(d) { freeHeur((d)); }
#elif USE_HEUR == 0
    #define HEUR_HEURISTIC(d) { hcol = heuristic2((d)); }
    #define HEUR_INIT(d, heur_max)
    #define HEUR_DEC(d, dcol, index, heur)
    #define HEUR_INC(d, dcol, index, heur)
    #define HEUR_FREE(d)
#else
    /* checkConfig() should assert USE_HEUR is 0 or 1 */
#endif

/*
    used for list memmory allocation
    starting capacity must be >= 1
    growth factor must be > 1
*/
#define STARTING_CAP 1
#define GROWTH_FACTOR 2

/*
    buffer size for printing
    no error checking for this
*/
#define BUFSIZE 100

#endif