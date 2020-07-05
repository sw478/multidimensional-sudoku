# sudoku

	use the bash script

longest recorded time to create a random fully populated board of varying sizes:
(rows are selected nondeterministically)
3x3: 0.05 sec
4x4: 0.14 sec
4x5: 0.26 sec
5x5: 0.6 sec
5x6: 18 sec
6x6: 5 mins
7x7: not yet tried

time to create a canonical fully populated board of varying sizes:
(rows are selected deterministically)
3x3: 0.035 sec
4x4: 0.13 sec
5x5: 0.48 sec
5x6: 1.1 sec
6x6: 0.9 sec
6x7: 1.2 sec
7x7: 2.5 sec
7x8: 12 sec
8x8: more than 10 mins 

	version 0.4

deleted the recursive add number of children code, a costly mistake
rewrote matrix structure initialization code, greatly increasing
speed of creation. new code first places coordinates into a list,
quicksorts by rows, then creates row headers and connects row pointers together
same for cols, sorts by cols, then creates necessary connections
random boards of sizes 5x5 or smaller can be created in less than a second
but 6x6 boards can take more than a few minutes to create and vary in
length of time to create.
current bottleneck is the heuristic function: takes O(n) time, where
n is number of columns currently uncovered in the matrix.
planning on implementing a minheap or something like that

	version 0.3

added ability to generate fully populated boards
current bottleneck is the initial creation of the dancing link structure
pratically, the structure would only be created once and used many times

	version 0.2

removed dfs and genetic, cleaned up in general.
adding sudoku generator
can display all solutions in matrix format or normal format
(printSingleSol and printSingleSol2)

	version 0.1

reconfigured dancing links algorithm to handle boards of different sizes and
dimensions and added appropriate tests

added todo.txt that lists short term and long term goals for this project

	version 0.0

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
