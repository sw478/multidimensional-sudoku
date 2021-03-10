# sudoku

This project features a sudoku solver written in C. C was chosen since a good portion of this code takes advantage
of pointer logic. Using Donald Knuth's Algorithm X and a dancing links data structure, we are able to solve this
exact cover problem. Since the algorithm and data structure are meant for exact cover problems in general, this
program can be used to solve those other problems, such as polynomino tiling and the n-queens problem.

* https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X
* https://en.wikipedia.org/wiki/Dancing_Links
* https://en.wikipedia.org/wiki/Exact_cover

![seppuku](images/sudoku_seppuku.png)

## process (sudoku)

The program reads from a file of a sudoku to solve, as well as a matrixFile that contains coordinates for the
dancing links matrix. A couple things are setup to help the program run, then Algorithm X is run on the matrix.
In the process, a solution tree is created, which identifies the solution(s).

To use this program to solve another exact cover problem, you can just swap out and replace some functions for it
to work, like how n-queens is currently implemented.

## possible project paths to take

use unavoidable hitting sets to generate sudokus\
enable processing boards of higher dimensions, ex. 2x2x2, 2x3x4, 3x3x3x3\
create an ML model to uncover and identify features of essentially different sudoku boards\
write side program to enumerate polynominos
