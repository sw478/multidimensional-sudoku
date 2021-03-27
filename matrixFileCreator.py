import sys
from numpy import prod

def initMatrixFile_Sudoku(matrixFile, dim):
    f = open(matrixFile, "w")
    n = len(dim)

    containerSize = prod(dim)
    sudokuSize = pow(containerSize, n)

    if(containerSize == 0):
        return

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
        iSpan = [0]*n
        dividend = iSudoku
        for idim in range(n):
            span[idim] = dividend % containerSize # index in span
            dividend //= containerSize

            iBoxSpan = span[idim] // dim[idim] # box index of this span
            iBox += iBoxSpan * boxMult[idim] # box index
        
        #print("iSudoku: %2d iBox: %d" % (iSudoku, iBox))

        for idim in range(n):
            span2 = span[:]
            del span2[idim]
            
            for j in range(len(span2)):
                iSpan[idim] += span2[j] * spanMult[j] # span index

        for inum in range(containerSize):
            mrow = iSudoku * containerSize + inum

            mcol[0] = iSudoku
            mcol[1] = sudokuSize + (iBox * containerSize) + inum
            for idim in range(n):
                mcol[idim+2] = (sudokuSize * (2 + idim)) + iSpan[idim] * containerSize + inum

            for iConstraint in range(nConstraints):
                f.write("%d %d\n" % (mrow, mcol[iConstraint]))

    rmax = containerSize * sudokuSize
    cmax = sudokuSize * (2 + n)

    #print("rmax: " + repr(rmax))
    #print("cmax: " + repr(cmax))

    f.close()

"""
def convert():
    f = open("sudokuConvert.txt", "r")
    lines = f.read().split("\n")
    f.close()
    
    #for line in lines:
    #    print(line, end="\n")

    newLines = []
    for line in lines:
        newLines += line.split(" ")

    for i, val in enumerate(newLines):
        if(i % 32 == 0 and i != 0):
            print("")
        print(val, end=" ")

    f = open("sudokuFile.txt", "w")
    dim = [2, 2, 2]
    f.write("%s\n" % str(len(dim)))
    for idim in dim:
        f.write("%s\n" % str(idim))

    for val in newLines:
        if(val == '.'):
            f.write("0\n")
        else:
            f.write("%s\n" % val)

    f.close()
"""

def matrixFile():
    matrixFile = sys.argv[1]
    dim = [int(idim) for idim in sys.argv[2:]]
    initMatrixFile_Sudoku(matrixFile, dim)

if __name__ == "__main__":
    matrixFile()
    #convert()