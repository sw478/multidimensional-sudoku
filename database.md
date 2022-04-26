# Database Implementation

Some of the files written and read from can be better implemented using a database.
This has not been implemented yet.

## sudokuFile & solutionFile

* Ordered list of integers
* sudokuFile is a partial sudoku, solutionFile is a full sudoku
* Might need metadata (n, dims)
  * Could also use other symmetric relations to group solutions together
* Could design a 2 layer tree since many sudokus can be generated from one solution:
  * Primary level: solutionFiles
  * Secordary level: sudokuFiles under solutionFiles where the solutions correspond to the puzzle
  * puzzles can also be graded by difficulty and sorted/partitioned as necessary
* Should store sudokus of equivalent container dimensions together
  * Since a [2, 3, 1] is symmetric to a [1, 2, 3], they can be grouped together
  * Use descending dim convention: so a [2, 3, 1] and a [1, 2, 3] would be stored under [3, 2, 1]

## matrixFile

* Unordered list of 2D integer coordinates
* Won't be many of these: one per ordered container dimensions
* Accessed frequently: once per DLX solve and DLX gen

## dimacsFile

* Unordered list of clauses (unordered list of signed integers)
* Minimal and extended version for a container dimension
* Shouldn't store the partial clauses with them
  * Information already available in sudokuFile and doesn't take long to append clauses to the end

## enumFile

* Unordered list of ordered lists of integers
* Might need metadata (n, dims, numSols)
* Won't be many of these
* Accessed infrequently
* Could just be a set of sudokuFiles
