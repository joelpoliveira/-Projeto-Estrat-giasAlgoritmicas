from sys import stdin, stdout


def readln():
    return stdin.readline().rstrip()
    
def outln(n, end = '\n'):
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
            print(self.numbers[n][i], end = ' ')
    
    def rotate(self, n):
        # rotate n times to the right. alguma maneira melhor para rotação?
        pass
    
    
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
                    print('', end = ' ')
                print('\n') if j == self.rows - 1 and i != self.rows - 1 else print()
    
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
