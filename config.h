#ifndef CONFIG_H
#define CONFIG_H

/*
    set to 1 for rows to be randomly chosen
    0 to choose rows in order
*/
#define RANDOMIZE_ROWS 1

/*
    set to 1 to use heur system

    will call decHeur or incHeur each time
    a doubly is covered oor uncovered
*/
#define USE_HEUR 1

/*
    used for list memmory allocation
    growth factor must be > 1
*/
#define STARTING_CAP 1
#define GROWTH_FACTOR 2

#endif