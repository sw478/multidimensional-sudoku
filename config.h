#ifndef CONFIG_H
#define CONFIG_H

/*
    0: selecting first column
    1: using heur system
    2: finding minCol each time
*/
#define USE_HEUR 1

#if USE_HEUR == 1
    #define HEUR_HEURISTIC(d) heuristic((d));
    #define HEUR_INIT(d, heur_max) initHeurList((d), (heur_max));
    #define HEUR_DEC(d, heur, amount) decHeur((d), (heur), (amount));
    #define HEUR_INC(d, heur, amount) incHeur((d), (heur), (amount));
    #define HEUR_FREE(d) freeHeur((d));
#elif USE_HEUR == 2
    #define HEUR_HEURISTIC(d) heuristic2((d));
    #define HEUR_INIT(d, heur_max)
    #define HEUR_DEC(d, heur)
    #define HEUR_INC(d, heur)
    #define HEUR_FREE(d)
#elif USE_HEUR == 0
    #define HEUR_HEURISTIC(d) (d)->root->right);
    #define HEUR_INIT(d, heur_max)
    #define HEUR_DEC(d, heur)
    #define HEUR_INC(d, heur)
    #define HEUR_FREE(d)
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
*/
#define BUFSIZE 1000

/*
    set to 2 to search for all solutions when solving
    set to 1 for normal settings (print and search all)
    set to 0 to stop at the first solution found

    solutions will not be printed if searching for all solutions
*/
#define SEARCH_ALL_SOLUTIONS 1

#if SEARCH_ALL_SOLUTIONS == 2
    #define ALGX_BREAK
    #define PRINT_ALL_SUDOKU_SOLS
#elif SEARCH_ALL_SOLUTIONS == 1
    #define ALGX_BREAK
    #define PRINT_ALL_SUDOKU_SOLS printSolutions_Sudoku(d);
#elif SEARCH_ALL_SOLUTIONS == 0
    #define ALGX_BREAK break;
    #define PRINT_ALL_SUDOKU_SOLS printSolutions_Sudoku(d);
#endif

/* used to count calls to algX and generate */
#define CALL_TRACKING_GEN 50
#define CALL_TRACKING_ALGX_GEN_1    1000000
#define THRESHOLD_TRY               20
#define THRESHOLD_ALGX_GEN_1        5000000
#define THRESHOLD_ALGX_GEN_2        50000

#endif