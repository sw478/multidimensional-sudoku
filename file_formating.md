# Project Specific File Formatting

## SUDOKU_FILE and SOLUTION_FILE Format

* First line: [n: number of dimensions]
* Next n lines: container dimensions
* Next sudokuSize lines: number symbols for each cell in the n-dimensional board
* Number Symbols are 1-indexed, 0 is reserved for an empty cell

## MATRIX_FILE

* findMatrixFile() will attempt to open the necessary matrix file
* Each line in this file corresponds to a matrix coordinate: row then column
* The filename should have a prefix of "dm", and the suffix should be the container dimensions
  * The prefix and container dimensions should be separated with an underscore
  * Container dimensions should be in decreasing order
  * Consequently, a matrix file will be the same for a [5, 3, 1] and a [3, 1, 5] sudoku as an example
  * Example: Matrix file name for a [2, 4, 3] will be "dm_4_3_2.txt"

## DIMACS_INPUT Format

* Conventional DIMACS file format
  * p cnf (# variables) (# clauses)
* Comment header will contain:
  * "mSudoku" title
  * (n)
  * (dim)

## ENUMERATE_FILE Format

* First line: [ne: number of sudokus]
* Second line: [n: number of dimensions]
* Next n lines: container dimensions
* Next ne lines: sudoku solution, one per line, number symbols separated by spaces
