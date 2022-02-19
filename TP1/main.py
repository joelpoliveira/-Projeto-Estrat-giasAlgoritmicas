from sys import stdin, stdout


def readln():
    return stdin.readline().rstrip()
    
def outln(n = '', end = '\n'):
    return stdout.write(str(n) + end)


class Piece:
    numbers = []
    
    def __init__(self):
        array = readln().split(' ')
        self.numbers = [[array[0], array[1]], [array[3], array[2]]]
        #print("Created: ", self.numbers)
    
    # just for debug
    def __str__(self):
        return ''.join(str(i) for i in self.numbers)
    
    def print_row(self, n):
        for i in range(2):
            outln(self.numbers[n][i], end = ' ')
    
    def rotate_right(self):
        temp = self.numbers
        self.numbers = [[temp[1][0], temp[0][0]], [temp[1][1], temp[0][1]]]
    
    def rotate_left(self):
        temp = self.numbers
        self.numbers = [[temp[0][1], temp[1][1]], [temp[0][0], temp[1][0]]]
    
    
class Board:    
    board = []
    rows = 0
    cols = 0
    
    def __init__(self, rows, cols):
        self.board = [[Piece() for j in range(cols)] for i in range(rows)]
        self.rows = rows
        self.cols = cols
     
    def print_board(self):
        for i in range(self.rows):
            for j in range(2):
                for x in range(self.cols):
                    self.board[i][x].print_row(j)
                    outln('', end = ' ')
                outln('\n') if j == self.rows - 1 and i != self.rows - 1 else outln()
    
    def swap(self):
        pass
    
    def solve(self):
        pass

    def check(self):
        pass
    
    # just for debug
    def test(self):
        self.board[0][0].rotate_right()
        self.print_board()


if __name__ == "__main__":
    # number of board to solve
    n = int(readln()) 
    
    # Number of pieces, Rows, Cols
    x = readln().split(' ')
    N, R, C = int(x[0]), int(x[1]), int(x[2])
    
    # Create Board
    Board = Board(R, C)
    
    Board.print_board()
    
    #Board.test()
    
    #Board.solve()
    #Board.check()
