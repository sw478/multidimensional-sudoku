import sys

"""
    matrix rows describe the possible placements of individual numbers in the
    sudoku (sudokuSize), as well as the possible values it can have (boxSize)

    matrix columns are the constraints, which for each unique number (position
    and value) is constrained with:
        1: no more than one number can be in a cell ()
        one of each value in range [0:boxSize] in each box (boxSize*)


    in a normal 2 dimensional sudoku, you have rows, columns, and boxes.
    in a generalized sudoku, the rows and columns become "span" and there
    are n number of span

    if a board is enumerated like this:
        0   1   2   3
        4   5   6   7
        8   9  10  11
       12  13  14  15
    the columns are looped through first, then the rows, so span[0] would
    be the columns and span[1] would be the rows

    boxes would be enumerate:
        b0  b1
        b2  b3
"""
def initMatrixFile_Sudoku(matrixFile, dim):
    f = open(matrixFile, "w")
    n = len(dim)

    containerSize = 1
    for idim in range(n):
        containerSize *= dim[idim]
    cs2 = pow(containerSize, 2)
    
    sudokuSize = pow(containerSize, n)

    span = [0]*n
    numBox = [1]*n

    for idim in range(n):
        numBox[idim] = containerSize // dim[idim] # number of boxes in this span
    print("numBox: " + repr(numBox))

    mult = [1]*n
    for idim in range(1,n):
        mult[idim] = numBox[idim-1] * mult[idim-1]
    print("mult: " + repr(mult))

    for iSudoku in range(sudokuSize):
        box = 0
        dividend = iSudoku
        print("%2d:" % iSudoku, end="")
        for idim in range(n):
            span[idim] = dividend % containerSize
            dividend //= containerSize

            boxIndex = span[idim] // dim[idim] # box index of this span
            print(" %d" % boxIndex, end="")
            box += boxIndex * mult[idim] # box index
        print("")
        
        #print("%d: %d" % (iSudoku, box))

        for inum in range(containerSize):
            mrow = iSudoku * containerSize + inum

            f.write("%d %d\n" % (mrow, iSudoku))
            f.write("%d %d\n" % (mrow, sudokuSize + (box * containerSize) + inum))
            for idim in range(n):
                f.write("%d %d\n" % (mrow, (sudokuSize*2 + cs2 * idim) + span[idim] * containerSize + inum))

    rmax = containerSize*sudokuSize
    cmax = sudokuSize*2 + cs2*n

    print("rmax: " + repr(rmax))
    print("cmax: " + repr(cmax))

    f.close()

if __name__ == "__main__":
    matrixFile = sys.argv[1]
    dim = [int(idim) for idim in sys.argv[2:]]
    initMatrixFile_Sudoku(matrixFile, dim)
