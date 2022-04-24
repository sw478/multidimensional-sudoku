# Project Requirements

* Use the "test.sh" bash script to run program
* "b:basic" to run sudoku related tasks

# SUDOKU_FILE and SOLUTION_FILE Format

* First line: [n: number of dimensions]
* Next n lines: container dimensions
* Next sudokuSize lines: number symbols for each cell in the n-dimensional board
* Number Symbols are 1-indexed, 0 is reserved for an empty cell

# DIMACS_INPUT Format

* Conventional DIMACS file format
  * p cnf (# variables) (# clauses)
* Comment header will contain:
  * "mSudoku" title
  * (n)
  * (dim)

# User Options 

* DLX Solve
* DLX Generate
* Enumerate
* ZChaff Solve
* ZChaff Generate

# DLX Solve

## Program args

./a.out s [SUDOKU_FILE] [SOLUTION_FILE]

## Goals

* Reads in sudoku from SUDOKU_FILE.
* Outputs solved sudoku to SOLUTION_FILE.

## Notes

* If SUDOKU_FILE doesn't exist, generate one first, or copy a sample puzzle from /samples into sudokuFile.

# DLX Generate

## Program args

./a.out g [SUDOKU_FILE] [SOLUTION_FILE] (dim)

## Goals

* Generate sudoku using container dimensions (dim).
* Outputs a single puzzle into SUDOKU_FILE and its solution to SOLUTION_FILE.

## Notes

* (dim) is a list of integers of the desired container dimensions.

# Enumerate

## Program args

./a.out e [SOLUTION_FILE] (dim)

## Goals

* Enumerate all solution sudokus of a given container dimension.

## Note

* Meant for analyzing all 288 [2, 2] sudokus.
* Not meant for any larger container dimensions.

# BSat Solve (Create Dimacs)

## Program args

./a.out z0 [SUDOKU_FILE] [DIMACS_INPUT]

## Goals

* Convert sudoku from SUDOKU_FILE to the dimacs input file.

# BSat Solve (Zchaff)

## Program args

./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]

## Goals

* Run zchaff on the dimacs file

# BSat Solve (Process Zchaff Results)

## Program args

./a.out z1 [SUDOKU_FILE] [SOLUTION_FILE] [DIMACS_OUTPUT]

## Goals

* Parse dimacs output file.
* Parse SUDOKU_FILE for container dimensions.
* Convert results to solved sudoku
* Writes solution to solution file

# BSat (Full Solve run)

## Program args

./a.out zs0 [SUDOKU_FILE] [DIMACS_INPUT]
./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]
./a.out zs1 [SUDOKU_FILE] [SOLUTION_FILE] [DIMACS_OUTPUT]

# BSat Generate (Create Dimacs)

## Program args

./a.out zg0 [DIMACS_INPUT] (dim)

## Goals

* Write empty sudoku constraints to the dimacs input file using given container dimensions.

# BSat Generate (Zchaff)

## Program args

./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]

## Goals

* Run zchaff on the dimacs file

# BSat Generate (Process Zchaff Results)

## Program args

./a.out zg1 [SOLUTION_FILE] [DIMACS_OUTPUT]

## Goals

* Parse dimacs output file.
* Convert results to solved sudoku
* Writes solution to solution file

# BSat (Full Generate run)

## Program args

./a.out zg0 [DIMACS_INPUT] (dim)
./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]
./a.out zg1 [SOLUTION_FILE] [DIMACS_OUTPUT]