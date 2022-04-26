# Project Requirements

## Program Options

* DLX Solve
* DLX Generate Full
* DLX Generate Partial
* DLX Enumerate
* ZChaff Solve
* ZChaff Generate

## Bash Script Options

* DLX Solve: s
* DLX Generate: g
  * DLX Generate Full
  * DLX Generate Partial
* DLX Enumerate: e
* ZChaff Solve: zs
  * ZChaff Solve 0
  * ZChaff (external)
  * ZChaff Solve 1
* ZChaff Generate: zg
  * ZChaff Generate 0
  * ZChaff (external)
  * ZChaff Generate 1
  * DLX Generate Partial

# DLX Solve

## Program args

* ./mSudoku s [SUDOKU_FILE] [SOLUTION_FILE]

## Goals

* Reads in sudoku from SUDOKU_FILE.
* Outputs solved sudoku to SOLUTION_FILE.

## Notes

* If SUDOKU_FILE doesn't exist, generate one first, or copy a sample puzzle from /samples into sudokuFile.

# DLX Generate Full

## Program args

* ./mSudoku gf [SOLUTION_FILE] (dim)

## Goals

* Generate full sudoku using container dimensions (dim).
* Outputs a single solution to SOLUTION_FILE.

# DLX Generate Partial

## Program args

* ./mSudoku gp [SOLUTION_FILE] [SUDOKU_FILE]

## Goals

* Generate partial sudoku using given SOLUTION_FILE.
* Outputs a single puzzle to SUDOKU_FILE.

# DLX Enumerate

## Program args

* ./mSudoku e [SOLUTION_FILE] (dim)

## Goals

* Enumerate all solution sudokus of a given container dimension.

## Note

* Meant for analyzing all 288 [2, 2] sudokus.
* Not meant for any larger container dimensions.

# Zchaff

## Program args

* ./zchaff [DIMACS_INPUT] > [DIMACS_OUTPUT]

## Goals

* Run zchaff on the dimacs file

# BSat Solve (Create Dimacs)

## Program args

* ./mSudoku zs0 [DIMACS_INPUT] [SUDOKU_FILE]

## Goals

* Convert sudoku from SUDOKU_FILE to the dimacs input file.

# BSat Solve (Process Zchaff Results)

## Program args

* ./mSudoku zs1 [DIMACS_OUTPUT] [SUDOKU_FILE] [SOLUTION_FILE]

## Goals

* Parse dimacs output file.
* Parse SUDOKU_FILE for container dimensions.
* Convert results to solved sudoku
* Writes solution to solution file

# BSat Generate (Create Dimacs)

## Program args

* ./mSudoku zg0 [DIMACS_INPUT] (dim)

## Goals

* Write empty sudoku constraints to the dimacs input file using given container dimensions.

# BSat Generate (Process Zchaff Results)

## Program args

* ./mSudoku zg1 [DIMACS_OUTPUT] [SOLUTION_FILE] (dim)

## Goals

* Parse dimacs output file.
* Convert results to solved sudoku
* Writes solution to solution file
* Use DLX to generate a puzzle