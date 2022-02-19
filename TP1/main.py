from sys import stdin, stdout


def read_all():
    return stdin.readlines()
    
def readln():
    return stdin.readline().rstrip()
    
def outln(n):
    return stdout.write(str(n) + "\n")


class Piece:
    numbers = []
    
    def __init__(self, rows, cols):
        #self.numbers = [[0 for j in range(rows)] for i in range(cols)]
        self.numbers = [1, 2, 3]
        
    def __str__(self):
        return f"{self.numbers}"


class Board:    
    board = []
    
    def __init__(self, rows, cols):
        self.board = [[Piece(rows, cols) for j in range(rows)] for i in range(cols)]
    
    def print_board(self):
        for i in self.board:
            print()
            for j in i:
                print(j, end='')
    
    def solve(self):
        pass

    def check(self):
        pass


if __name__ == "__main__":
    Board = Board(2, 2)
    Board.print_board()
    #Board.solve()
    #Board.check()
