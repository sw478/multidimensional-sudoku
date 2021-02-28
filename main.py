import random
import time
import sys
from operator import itemgetter

class Doubly():
    def __init__(self, drow, dcol):
        self.drow = drow
        self.dcol = dcol
        self.up = self.down = self.left = self.right = self
        self.hrow = self.hcol = self
        self.heur = None

class solTrie():
    def __init__(self):
        self.parent = self

    def setParent(self, parent):
        self.parent = parent

class Hide():
    def __init__(self):
        self.hrows = []
        self.num = 0
        self.filled = False

class Heur():
    def __init__(self, num):
        self.head = self.prev = self.next = self
        self.hprev = self.hnext = self
        self.num = num

    def incHeur(self):
        head = self.head
        self.num += 1
        self.next.prev = self.prev
        self.prev.next = self.next

        if(head.next is head and head.num != 0):
            temp = head.hprev
            head.hnext.hprev = head.hprev
            head.hprev.hnext = head.hnext
            head = temp

        head = head.hnext
        if(head.num != self.num):
            temp = Heur(self.num)
            temp.hprev = head.hprev
            temp.hnext = head
            head.hprev.hnext = temp
            head.hprev = temp
            head = temp
        self.next = head.next
        self.prev = head
        head.next.prev = self
        head.next = self
        self.head = head

    def decHeur(self):
        head = self.head
        self.num -= 1
        self.next.prev = self.prev
        self.prev.next = self.next

        if(head.next is head and head.num != 0):
            temp = head.hnext
            head.hnext.hprev = head.hprev
            head.hprev.hnext = head.hnext
            head = temp

        head = head.hprev
        if(head.num != self.num):
            temp = Heur(self.num)
            temp.hnext = head.hnext
            temp.hprev = head
            head.hnext.hprev = temp
            head.hnext = temp
            head = temp

        self.next = head.next
        self.prev = head
        head.next.prev = self
        head.next = self
        self.head = head

class Dance():
    def __init__(self, matrixFile, rmax, cmax):
        self.rmax = rmax
        self.cmax = cmax
        self.root = Doubly(rmax, cmax)
        self.xcol = self.xrow = self.root
        self.hideList = []
        self.solRoot = solTrie()
        self.csol = self.solRoot
        self.sols = []
        self.numSols = 0
        self.numCalls = 0
        self.heurRoot = None

        with open(matrixFile) as f:
            lines = f.read().split("\n")
        lines.remove("")

        initList = []
        for line in lines:
            line = line.split(" ")
            irow = line[0]
            icol = line[1]

            initList.append(Doubly(irow, icol))

        self.connectRows(initList)

    def setSudoku(self, sudoku):
        self.s = sudoku

    def connectRows(self, initList):
        hrow = self.root
        prev = -1

        # sort initList by rows in increasing order
        initList = sorted(initList, key=itemgetter(0))

        for cur in initList:
            irow = cur.drow
            if(irow != prev): # initialize new row header
                hnew = Doubly(irow, self.cmax)
                hnew.down = hrow.down
                hnew.up = hrow
                hnew.hcol = self.root
                hnew.hrow = hnew
                hrow.down.up = hnew
                hrow.down = hnew
                hrow = hnew
                self.root.drow += 1
                prev = irow
            cur.right = hrow
            cur.left = hrow.left
            cur.hrow = hrow
            hrow.dcol += 1
            hrow.left.right = cur
            hrow.left = cur

    def connectCols(self, initList):
        hcol = self.root
        prev = -1

        # sort initList by cols in increasing order
        initList = sorted(initList, key=itemgetter(1))

        for cur in initList:
            icol = cur.dcol
            if(icol != prev): # initialize new col header
                hnew = Doubly(self.rmax, icol)
                hnew.right = hcol.right
                hnew.left = hcol
                hnew.hcol = hnew
                hnew.hrow = self.root
                hcol.right.left = hnew
                hcol.right = hnew
                hcol = hnew
                self.root.dcol += 1
                prev = icol
            cur.down = hcol
            cur.up = hcol.up
            cur.hcol = hcol
            hcol.drow += 1
            hcol.up.down = cur
            hcol.up = cur

    def initHeurList(self):
        self.heurRoot = Heur(0)
        self.heurRoot.hcol = self.root

        hcol = self.root.right
        while(hcol != self.root):
            heur = Heur(hcol.drow - self.rmax)
            heur.hcol = hcol
            hcol.heur = heur

            head = self.heurRoot.hnext
            while(heur.num < head.num):
                head = head.hnext

            if(heur.num != head.num):
                temp = Heur(heur.num)
                temp.hprev = head.prev
                temp.hnext = head
                head.hprev.hnext = temp
                head.hprev = temp
                head = temp

            heur.next = head
            heur.prev = head.prev
            head.prev.next = heur
            head.prev = heur
            heur.head = head

            hcol = hcol.right

    def initHide(self):
        grid = self.s.board
        xy = self.s.xy
        num = 0

        xrow = self.root.down

        for igrid in range(self.s.gridSize):
            h = Hide()
            h.num = grid[igrid]
            self.hideList[igrid] = h
            h.hrows = [None for i in range(xy-1)]
            h.filled = 0

            if(grid[igrid] == 0):
                ihide = 0
                while(ihide < xy):
                    ihide += 1
                    xrow = xrow.down
                continue

            for ihide in range(xy-1):
                num = (xrow.drow % xy) + 1
                if(grid[igrid] == num):
                    continue
                h.hrows[ihide] = xrow
                xrow = xrow.down

            if(num != xy):
                xrow = xrow.down

    def hideAllCells(self):
        for igrid in range(self.s.gridSize):
            self.hideSingleCell(igrid)

    def unhideAllCells(self):
        for igrid in range(self.s.gridSize):
            self.unhideSingleCell(igrid)

    def hideSingleCell(self, igrid):
        h = self.hideList[igrid]
        xy1 = self.s.xy - 1

        if(h.num == 0):
            return 0
        if(h.filled):
            return 1

        for ihide in range(xy1):
            hrow = h.hrows[ihide]

            xrow = hrow.right
            while(xrow != hrow):
                xrow.up.down = xrow.down
                xrow.down.up = xrow.up
                xrow.hcol.dorw -= 1
                xrow.hcol.heur.decHeur()

                xrow = xrow.right

        h.filled = True

    def unhideSingleCell(self, igrid):
        h = self.hideList[igrid]
        xy1 = self.s.xy - 1

        if(h.num == 0):
            return 0
        if(not h.filled):
            return 1

        for ihide in range(xy1):
            hrow = h.hrows[ihide]

            xrow = hrow.right
            while(xrow != hrow):
                xrow.up.down = xrow
                xrow.down.up = xrow
                xrow.hcol.drow += 1
                xrow.hcol.heur.incHeur()

                xrow = xrow.right

        h.filled = False

    def coverRowHeaders(self):
        hrow = self.root.down
        while(hrow != self.root):
            hrow.right.left = hrow.left
            hrow.left.right = hrow.right

            hrow = hrow.down

    def uncoverRowHeaders(self):
        hrow = self.root.down
        while(hrow != self.root):
            hrow.right.left = hrow
            hrow.left.right=  hrow

            hrow = hrow.down

    def savePuzzle(self):
        for igrid in range(self.s.gridSize):
            if(not self.hideList[igrid].filled):
                self.s.grid[igrid] = 0

    def printSolutions(self):
        res = ""

        for i in range(self.numSols):
            res += "\n\nsol %d: " % (i + 1)
            res += self.printSingleSol2(self.sols[i])
        res += "\n\n"

        return res

    def printSingleSol(self, sol):
        res = ""
        if(sol == self.solRoot):
            return

        res += self.printColHeaders()

        cur = sol
        while(cur.parent != cur):
            res += "%3d: " % (cur.row.drow)
            pcol = 0
            irow = 0
            xcol = cur.row.right
            while(pcol < cur.row.left.dcol):
                while(pcol < xcol.dcol):
                    res += "."
                    pcol += 1
                res += "X"

                xcol = xcol.right
                irow += 1
                pcol += 1

            while(pcol < self.cmax):
                res += "."
                pcol += 1
            res += "\n"

            cur = cur.parent
        res += "\n"

        return res

    def printSingleSol2(self, sol):
        xy = self.s.xy
        if(not sol.row):
            return
        grid = [0]*self.cmax

        cur = sol
        while(cur.parent != cur):
            drow = cur.row.drow
            num = drow % xy
            igrid = drow // xy
            grid[igrid] = num + 1

            cur = cur.parent

        return printBoard(grid, self.s.x, self.s.y)

    def printColHeaders(self):
        res = ""

        res += "     "
        for pcol in range(self.cmax):
            res += "%d" % (pcol % 10)
        res += "\nX's: "

        pcol = 0
        xcol = self.root.right
        while(xcol != self.root):
            while(pcol < xcol.dcol):
                pcol += 1
                res += "0"
            res += "%d" % (xcol.drow - self.rmax)

            xcol = xcol.right
            pcol += 1

        for pcol in range(self.cmax):
            res += "0"
        res += "\n"

        return res

    def __repr__(self):
        res = ""
        pcol = 0

        res += self.printColHeaders()

        xrow = self.root.down
        while(xrow != self.root):
            nrow = xrow.dcol - self.cmax
            if(nrow == 0):
                continue
            res += "%3d: " % (xrow.drow)
            pcol = 0
            irow = 0
            xcol = xrow.right
            while(irow < nrow):
                while(pcol < xcol.dcol):
                    pcol += 1
                    res += "."
                res += "X"

                xcol = xcol.right
                irow += 1
                pcol += 1

            while(pcol < self.cmax):
                pcol += 1
                res += "."
            res += "\n"

            xrow = xrow.down
        res += "\n"

class Sudoku():
    def __init__(self, grid, x, y):
        self.grid = grid
        self.x = x
        self.y = y
        self.xy = x*y
        self.gridSize = self.xy*self.xy

    def __repr__(self):
        return printBoard(self.grid, self.x, self.y)

def initMatrixFileSudoku(x, y):
    xy = x*y
    gridSize = xy*xy
    fileName = "dance/ds0r%dc%d" % (y, x)
    f = open(fileName, "w")

    for igrid in range(gridSize):
        sr = igrid // xy
        sc = igrid % xy
        sb = (sr // y) * y + sc // x

        for inum in range(xy):
            icol = []
            irow = igrid * xy + inum
            icol.append(igrid)
            icol.append(inum + sr*xy + gridSize)
            icol.append(inum + sc*xy + gridSize)
            icol.append(inum + sb*xy + gridSize)

            for i in range(4):
                f.write("%d %d\n" % (irow, icol[i]))

    rmax = xy*gridSize
    cmax = 4*gridSize

    f.close()
    return rmax, cmax

def printBoard(grid, x, y):
    res = ""
    xy = x * y
    for row in range(xy):
        if row % y == 0:
            res += "\n"
        for col in range(xy):
            if (col % x == 0):
                res += " "
            value = grid[row * xy + col]
            if (value != 0):
                res += " %2d" % int(value)
            else:
                res += " __"
        res += "\n"

    return res

def readSudokuBoard(fileName):
    with open(fileName) as f:
        board = f.read().split('\n')
    x_y = board[0].split(" ")

    x = int(x_y[0])
    y = int(x_y[1])
    board = board[1:]
    board.remove("")
    print(board)
    return Sudoku(board, x, y)

def main():
    mode = int(sys.argv[1])
    s = readSudokuBoard(sys.argv[2])

    # mode == 1 if solving sudoku
    # mode == 0 if generating (doesn't work yet)
    if(mode == 1):
        print(s)

    rmax, cmax = initMatrixFileSudoku(s.x, s.y)
    maxtrixFile = "dance/ds0r%dc%d" % (s.y, s.x)
    d = Dance(maxtrixFile, rmax, cmax)
    d.setSudoku(s)
    d.initHeurList()

    if(mode == 1):
        d.initHide()
        d.hideAllCells()

    d.coverRowHeaders()

    d.uncoverRowHeaders()

    if(mode == 1):
        d.unhideAllCells()

    d.printSolutions()
    d.savePuzzle()

if __name__ == "__main__":
    main()
