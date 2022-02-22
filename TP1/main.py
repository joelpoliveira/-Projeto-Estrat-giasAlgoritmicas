from sys import stdin, stdout
from collections import deque

def read_all():
    return stdin.readlines()

def readln():
    return stdin.readline().rstrip()
    
def outln(n = '', end = '\n'):
    return stdout.write(str(n) + end)


class Piece:    
    def __init__(self, array):
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

    def match_up(self, piece):
        return (self.number[0][0] == piece.number[1][0]) and (self.number[0][1] == piece.number[1][1]) 

    def match_left(self, piece):
        return (self.number[0][0] == piece.number[0][1]) and (self.number[1][0] == piece.number[1][0]) 
    
    
class Board:    
    def __init__(self, rows, cols, first_piece):
        self.board = [[None for i in range(cols)] for j in range(rows)]
        self.board[0][0] = first_piece
        self.rows = rows
        self.cols = cols
        self.next_empty = 1
     
    def print_board(self):
        for i in range(self.rows):
            for j in range(2):
                for x in range(self.cols):
                    if self.board[i][x] !=None:
                        self.board[i][x].print_row(j)
                    else:
                        outln("|", end = ' ')
                    outln('', end = ' ')
                outln('\n') if j == self.rows - 1 and i != self.rows - 1 else outln()
    
    def get_next(self):
        row = self.next_empty//self.rows
        col = self.next_empty%self.cols
        return row,col

    def get_now(self):
        row = (self.next_empty - 1)//self.rows
        col = (self.next_empty - 1)%self.cols
        return row,col

    def insert(self, Piece):
        r,c = self.get_next()
        print(r, c)
        self.board[r][c] = Piece
        self.next_empty+=1

    def swap(self):
        pass
    
    def solve(self):
        pass

    def check(self):
        r,c = self.get_now()
        piece_now = self.board[r][c]

        if r - 1 < 0:
            if piece_now.match(self.board[r][c-1]):
                return True
            return False
        else:
            if piece_now.match(self.board[r-1][c]) and piece_now.match(self.board[r][c-1]):
                return True
            return False


    
    # just for debug
    def test(self):
        self.board[0][0].rotate_right()
        self.print_board()


if __name__ == "__main__":
    # number of board to solve
    n = int(readln()) 
    for _ in range(n):
        stack = deque()
        # Number of pieces, Rows, Cols
        N, R, C = list(map(int, readln().split()))

        first_piece = Piece(readln().split())
        # Create Board
        Board = Board(R, C, first_piece)
        for _2 in range(N - 1):
            stack.append( Piece( readln().split() ) )

        while True:
            if stack:
                piece = stack.pop()

            else:
                break
        Board.print_board()
    
    #Board.test()
    
    #Board.solve()
    #Board.check()
