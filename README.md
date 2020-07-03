# sudoku

	version 1.2

removed dfs and genetic, cleaned up in general.
adding sudoku generator
can display all solutions in matrix format or normal format
(printSingleSol and printSingleSol2)

	version 1.1

reconfigured dancing links algorithm to handle boards of different sizes and
dimensions and added appropriate tests

added todo.txt that lists short term and long term goals for this project

	version 1.0

reads in a text file containing 81 digits 0-9 representing a classic 9x9
sudoku board

currently solves boards using 3 approaches:

backtracking:
uses depth first search to fill in digits, and when a contradiction appears, the
algorithm backtracks to the last valid cell placement.

genetic algorithm:
doesn't work that well for solving sudoku puzzles, but was written mainly for
personal practice. user can control size of generation and percent of survivors.
scoring is based on how many conditions (row/col/box) are broken.

exact cover / dancing links:
since sudoku can be reduced to an exact cover problem, I created a generalized
dancing links structure that could be used for other exact cover problems, such
as pentominos.

	en.wikipedia.org/wiki/Exact_cover
