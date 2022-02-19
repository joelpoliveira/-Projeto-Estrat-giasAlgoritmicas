from sys import stdin, stdout


# def read_all():
#     return stdin.readlines()
    
def readln():
    return stdin.readline().rstrip()
    
def outln(n, end = '\n'):
    return stdout.write(str(n) + end)


class Piece:
    numbers = []
    rows = 0
    cols = 0
    
    def __init__(self, rows, cols):
        self.numbers = [[j for j in range(cols)] for i in range(rows)]
        self.rows = rows
        self.cols = cols
        #print("Created: ", self.numbers)
    
    def print_row(self, n):
        for i in range(self.cols):
            print(self.numbers[n][i], end = ' ')
    
    # just for debug
    def __str__(self):
        return ''.join(str(i) for i in self.numbers)


class Board:    
    board = []
    rows = 0
    cols = 0
    
    def __init__(self, rows, cols):
        self.board = [[Piece(rows, cols) for j in range(cols)] for i in range(rows)]
        self.rows = rows
        self.cols = cols
     
    def print_board(self):
        # iterate each piece
        for i in range(self.rows):
            for j in range(self.cols):
                #  iterate each row of a piece
                if self.rows == 1:
                        self.board[i][j].print_row(0)
                        print('', end = '  ')
                else:
                    for x in range(self.cols):
                        self.board[i][j].print_row(x)
                        print('', end = '  ')
                    print('\n') if j == self.cols - 1 and i != self.rows - 1 else print()
    
    def solve(self):
        pass

    def check(self):
        pass


if __name__ == "__main__":
    # number of board to solve
    n = int(readln()) 
    
    # Number of pieces, Rows, Cols
    x = readln().split(' ')
    N, R, C = int(x[0]), int(x[1]), int(x[2])
    
    # Create Board
    Board = Board(R, C)
    
    Board.print_board()
    #Board.solve()
    #Board.check()
