# multidimensional sudoku

This project is based off of the main sudoku project, as most of the code is reusable. It generalizes the ideas of a traditional 9x9 sudoku to sudokus of any dimension, of any size.

## general notes

* Use the "test.sh" bash script to run program
  * "b:basic"
* Sample sudokus are in /samples.
* Using [zchaff](https://www.princeton.edu/~chaff/zchaff.html#:~:text=zChaff%20is%20an%20implementation%20of,variables%20and%2010%20million%20clauses.) for bSat. bSat usage is only loosely integrated at the moment for testing.
* The generator can write to the sudokuFile for the solver, or you can replace the sudokuFile with your own sudoku puzzle to solve.

## user options (test.sh)

* Solve: solve sudoku located in sudokuFile. If sudoku doesn't exist, generate one first, or copy a sample puzzle from /samples into sudokuFile.
* Generate: generate sudoku, input is just container dimensions.
* Enumerate: enumerate all sudokus of a given dimensions. Don't expect this finish on anything other than small sudokus (largest: [2, 2], [3, 2]). Meant for analyzing all 288 [2, 2] sudokus.
* bSat: create a dimacs file for inputed dimensions, then run zchaff on dimacs file. Doesn't do anything other than spit out SAT/UNSAT. Plan to expand on this functionality.

## process (dancing links)

### solver

* The program reads from a file containing the sudoku (sudokuFile) to solve,
* as well as a file (matrixFile) that contains coordinates for the dancing links matrix.
* If the correct matrixFile does not exist yet, matrixFileCreator.py can be used to create it.
* The generator can write to the sudokuFile for the solver to solve.
* Algorithm X is run on the matrix.
* A solution tree is created, identifying the solution(s).

### generator

* The program reads from a file (matrixFile) that contains coordinates for the dancing links matrix.
* If the correct matrixFile does not exist yet, matrixFileCreator.py can be used to create it.
* Algorithm X is run on the matrix to pick a single random solution (full sudoku).
* Hide is initialized with the solved board.
* Generate() is run to create a puzzle (partial sudoku) with the solved board as the only solution.
* Newly created puzzle is saved in sudokuFile.

