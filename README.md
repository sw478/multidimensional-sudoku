# Multidimensional Sudoku README

This project is based off of the main sudoku project, as most of the code is reusable. It generalizes the ideas of a traditional 9x9 sudoku to sudokus of any dimension, of any size.

## general notes

* Use the "test.sh" bash script to run program: "b:basic".
* Sample sudokus are in /samples.
* Using [zchaff](https://www.princeton.edu/~chaff/zchaff.html#:~:text=zChaff%20is%20an%20implementation%20of,variables%20and%2010%20million%20clauses.) for bSat.
* The generator can write to the sudokuFile for the solver, or you can replace the sudokuFile with your own sudoku puzzle to solve.

## user options (test.sh)

* DLX Solve: solve sudoku located in sudokuFile. If sudoku doesn't exist, generate one first, or copy a sample puzzle from /samples into sudokuFile.
* DLX Generate: generate a full sudoku, generate puzzle from solution. Input is just container dimensions.
* DLX Enumerate: enumerate all sudokus of a given dimensions. Don't expect this finish on anything other than small sudokus (largest: [2, 2], [3, 2]). Meant for analyzing all 288 [2, 2] sudokus.
* ZChaff Solve: same functionality as DLX Solve, but using bSat.
* ZChaff Generate: same functionality as DLX Generate, but using bSat for full sudoku generation, and DLX for puzzle generation.