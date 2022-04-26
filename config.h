#ifndef CONFIG_H
#define CONFIG_H

/*
    0: selecting first column
    1: using heur system
    2: finding minCol each time
*/
#define USE_HEUR 0

#if USE_HEUR == 0
    #define HEUR_HEURISTIC(d) ((d)->root->right);
    #define HEUR_INIT(d, heur_max)
    #define HEUR_DEC(d, heur, amount)
    #define HEUR_INC(d, heur, amount)
    #define HEUR_FREE(d)
#elif USE_HEUR == 1
    #define HEUR_HEURISTIC(d) heuristic((d));
    #define HEUR_INIT(d, heur_max) initHeurList((d), (heur_max));
    #define HEUR_DEC(d, heur, amount) decHeur((d), (heur), (amount));
    #define HEUR_INC(d, heur, amount) incHeur((d), (heur), (amount));
    #define HEUR_FREE(d) freeHeur((d));
#elif USE_HEUR == 2
    #define HEUR_HEURISTIC(d) heuristic2((d));
    #define HEUR_INIT(d, heur_max)
    #define HEUR_DEC(d, heur, amount)
    #define HEUR_INC(d, heur, amount)
    #define HEUR_FREE(d)
#endif

/*
    used for dynamic list growth
    starting capacity must be >= 1
    growth factor must be > 1
*/
#define STARTING_CAP 1
#define GROWTH_FACTOR 2

/*
    buffer size of strings for printing
*/
#define BUFSIZE 1000

/* used to count calls to algX and generate */
/* TO DO: thresholds should be a factor of sudokuSize and dimensions */
#define CALL_TRACKING_GEN           500
#define THRESHOLD_TRY               1000
#define THRESHOLD_ALGX_GEN_1        10000000
#define THRESHOLD_ALGX_GEN_2        1000000
#define THRESHOLD_GEN_FACTOR        10000
#define CALL_TRACKING_ALGX_GEN_1    5000000

#endif