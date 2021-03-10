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

use unavoidable hitting sets to generate sudokus
enable processing boards of higher dimensions, ex. 2x2x2, 2x3x4, 3x3x3x3\
create an ML model to uncover and identify features of essentially different sudoku boards
write side program to enumerate polynominos

## unistd_io.h

Create a file of this name that includes io.h if running on windows or unistd.h if on linux

## logic

Given a list of coordinates for an exact cover matrix, solution(s) are found by finding a set of disjoint rows with coordinates that
span all columns.

Example from wikipedia:

        1   2   3   4   5   6   7
    A   X   .   .   X   .   .   X
    B   X   .   .   X   .   .   .
    C   .   .   .   X   X   .   X
    D   .   .   X   .   X   X   .
    E   .   X   X   .   .   X   X
    F   .   X   .   .   .   .   X

If the above is the input matrix, a solution would be the set of rows { B, D, F }:

        1   2   3   4   5   6   7
    B   X   .   .   X   .   .   .
    D   .   .   X   .   X   X   .
    F   .   X   .   .   .   .   X

As you can see, each column has exactly one X.

For sudokus, we can model them by seeing that for a board with:

    box dimensions = x & y,
    row, col, and box size = xy
    gridSize = xy*xy
    
There are 4 types of "constraints": numbers 1 through xy must be in a row, column, and box, and no more than one number can be in a
single cell. These constraints are what define the matrix's columns (4\*gridSize), and its rows represent the possible placement
of a number and the value of the number itself (xy\*gridSize).

Solving the matrix in this way allows us to easily and quickly find a solution for a sudoku puzzle.