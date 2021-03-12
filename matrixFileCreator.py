import sys

solRows = [0, 3, 5, 6, 9, 10, 12, 15]

def initMatrixFile_Sudoku(matrixFile, dim):
    f = open(matrixFile, "w")
    n = len(dim)

    containerSize = 1
    for idim in range(n):
        containerSize *= dim[idim]
    
    sudokuSize = pow(containerSize, n)

    span = [0]*n
    iSpan = [0]*n

    boxMult = [1]*n
    spanMult = [1]*n
    for idim in range(1,n):
        boxMult[idim] = containerSize // dim[idim-1] * boxMult[idim-1]
        spanMult[idim] = containerSize * spanMult[idim-1]

    nConstraints = n + 2
    mcol = [0]*nConstraints
    for iSudoku in range(sudokuSize):
        iBox = 0
        dividend = iSudoku
        for idim in range(n):
            span[idim] = dividend % containerSize # index in span
            dividend //= containerSize

            iBoxSpan = span[idim] // dim[idim] # box index of this span
            iBox += iBoxSpan * boxMult[idim] # box index

        iSpan = [0]*n
        for idim in range(n):
            span2 = span.copy()
            del span2[idim]
            
            for j in range(len(span2)):
                iSpan[idim] += span2[j] * spanMult[j]

        for inum in range(containerSize):
            mrow = iSudoku * containerSize + inum

            mcol[0] = iSudoku
            mcol[1] = sudokuSize + (iBox * containerSize) + inum
            for idim in range(n):
                mcol[idim+2] = (sudokuSize * (2 + idim)) + iSpan[idim] * containerSize + inum

            for iConstraint in range(nConstraints):
                f.write("%d %d\n" % (mrow, mcol[iConstraint]))

            continue
            if mrow in solRows:
                print("%2d: " % mrow, end="")
                for iConstraint in range(nConstraints):
                    print(" %2d" % mcol[iConstraint], end="")
                print("")

    rmax = containerSize * sudokuSize
    cmax = sudokuSize * (2 + n)

    print("rmax: " + repr(rmax))
    print("cmax: " + repr(cmax))

    f.close()

if __name__ == "__main__":
    matrixFile = sys.argv[1]
    dim = [int(idim) for idim in sys.argv[2:]]
    initMatrixFile_Sudoku(matrixFile, dim)
