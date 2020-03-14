# sudoku

reads in a text file containing 81 digits 0-9 representing a classic 9x9
sudoku board

currently solves boards using 3 approaches:

backtracking:
uses dfs to fill in digits, and when a contradiction appears, the
algorithm backtracks to the last valid cell placement.

genetic algorithm:
doesn't work that well for solving sudoku puzzles, but was written mainly for
personal practice. user can control size of generation and percent of survivors.
scoring is based on how many conditions (row/col/box) are broken.

exact cover / dancing links:
since sudoku can be reduced to an exact cover problem, I created a generalized
dancing links structure that could be used for other exact cover problems, such
as pentominos. Currently in the works.
