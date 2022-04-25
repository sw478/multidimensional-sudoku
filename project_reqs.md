# Project Requirements

* DLX Solve
* DLX Generate
* DLX Enumerate
* ZChaff Solve
* ZChaff Generate

# DLX Solve

## Program args

* ./mSudoku s [SUDOKU_FILE] [SOLUTION_FILE]

## Goals

* Reads in sudoku from SUDOKU_FILE.
* Outputs solved sudoku to SOLUTION_FILE.

## Notes

* If SUDOKU_FILE doesn't exist, generate one first, or copy a sample puzzle from /samples into sudokuFile.

# DLX Generate

## Program args

* ./mSudoku g [SUDOKU_FILE] [SOLUTION_FILE] (dim)

## Goals

* Generate sudoku using container dimensions (dim).
* Outputs a single puzzle into SUDOKU_FILE and its solution to SOLUTION_FILE.

## Notes

* (dim) is a list of integers of the desired container dimensions.

# DLX Enumerate

## Program args

* ./mSudoku e [SOLUTION_FILE] (dim)

## Goals

* Enumerate all solution sudokus of a given container dimension.

## Note

* Meant for analyzing all 288 [2, 2] sudokus.
* Not meant for any larger container dimensions.

# BSat Solve (Create Dimacs)

## Program args

* ./mSudoku zs0 [DIMACS_INPUT] [SUDOKU_FILE]

## Goals

* Convert sudoku from SUDOKU_FILE to the dimacs input file.

# BSat Solve (Zchaff)

## Program args

* ./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]

## Goals

* Run zchaff on the dimacs file

# BSat Solve (Process Zchaff Results)

## Program args

* ./mSudoku zs1 [DIMACS_OUTPUT] [SUDOKU_FILE] [SOLUTION_FILE]

## Goals

* Parse dimacs output file.
* Parse SUDOKU_FILE for container dimensions.
* Convert results to solved sudoku
* Writes solution to solution file

# BSat (Full Solve run)

## Program args

* ./mSudoku zs0 [DIMACS_INPUT] [SUDOKU_FILE]
* ./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]
* ./mSudoku zs1 [DIMACS_OUTPUT] [SUDOKU_FILE] [SOLUTION_FILE]

# BSat Generate (Create Dimacs)

## Program args

* ./mSudoku zg0 [DIMACS_INPUT] (dim)

## Goals

* Write empty sudoku constraints to the dimacs input file using given container dimensions.

# BSat Generate (Zchaff)

## Program args

* ./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]

## Goals

* Run zchaff on the dimacs file

# BSat Generate (Process Zchaff Results)

## Program args

* ./mSudoku zg1 [DIMACS_OUTPUT] [SUDOKU_FILE] [SOLUTION_FILE] (dim)

## Goals

* Parse dimacs output file.
* Convert results to solved sudoku
* Writes solution to solution file

# BSat (Full Generate run)

## Program args

* ./mSudoku zg0 [DIMACS_INPUT] (dim)
* ./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]
* ./mSudoku zg1 [DIMACS_OUTPUT] [SUDOKU_FILE] [SOLUTION_FILE] (dim)