from sys import stdin, stdout, setrecursionlimit
from time import time

setrecursionlimit(2500)

def read_all():
    return stdin.readlines()

def readln():
    return stdin.readline().rstrip()


def outln(n="", end="\n"):
    return stdout.write(str(n) + end)

class Piece:
    left = None
    right = None
    up = None
    down = None
    values = []

    def __str__(self):
        return (
            "u:" + str(self.up) +
            " r:" + str(self.right) +
            " d:" + str(self.down) + 
            " l:" + str(self.left) 
        )


def print_board(board, r, c):
    
    for i in range(r):
        line = ''
        for k in range(2):
            for j in range(c):
                if k == 0:
                    line += str(board[i][j].up) + ''
                else:
                    line += str(board[i][j].down) + ''
                line += '\n' if j == c - 1 else '  '
        outln(line)
        
    #outln("\n", end="")


def rotate(piece):
    rotated_piece = Piece()
    temp = piece.values
    rotated_piece.up = (temp[3], temp[0])
    rotated_piece.right = (temp[0], temp[1])
    rotated_piece.down = (temp[2], temp[1])
    rotated_piece.left = (temp[3], temp[2])
    return rotated_piece


def solve(board, pieces, r, c):
    pieces_dict = {}
    
    # apenas guardar right and down pela maneira como as peças sao inseridas na board. da esquerda para direita e de cima para baixo
    for p in pieces:
        temp = p
        for i in range(3):
            pieces_dict['u' + str(temp.up)] = p
            pieces_dict['r' + str(temp.right)] = p
            pieces_dict['d' + str(temp.down)] = p
            pieces_dict['l' + str(temp.left)] = p
            
            temp = rotate(p)
    
    for key, value in pieces_dict.items():
        print(key + ' -> ' + str(value))
    
    # Fazer board
    for i in range(r):
        for j in range(c - 1):
            if i == 0:
                # se estivermos na 1º linha
                print(f'({i}, {j}) ===', pieces_dict['l' + str(board[i][j].right)])
                board[i][j + 1] = pieces_dict['l' + str(board[i][j].right)]
            else:
                if j == 0: # linha n, coluna 0 -> nenhuma peça do lado esquerdo
                    print(f'({i}, {j}) ===', pieces_dict['u' + str(board[i-1][j].down)])
                    board[i][j] = pieces_dict['u' + str(board[i-1][j].down)]
                else:
                    pass
    
    print('\n')
    print_board(board, r, c)
            

if __name__ == "__main__":
    for _ in range(int(readln())):
        N, R, C = list(map(int, readln().split()))
        
        pieces = []
        
        for i in range(N):
            array = list(map(int, readln().split()))
            
            new_piece = Piece()
            new_piece.values = array
            new_piece.up = (array[0], array[1])
            new_piece.right = (array[1], array[2])
            new_piece.down = (array[3], array[2])
            new_piece.left = (array[0], array[3])
            
            pieces.append(new_piece)
    
        # Create Board
        board = [[Piece() for _ in range(C)] for __ in range(R)]
        #board = [[None for _ in range(C)] for __ in range(R)]
        board[0][0] = pieces[0]
        
        # Colocar todas as peças na board para debug
        # for i in range(R):
        #     for j in range(C):
        #         array = list(map(int, readln().split()))
        #         new_piece = Piece()
        #         new_piece.up = (array[0], array[1])
        #         new_piece.right = (array[1], array[2])
        #         new_piece.down = (array[3], array[2])
        #         new_piece.left = (array[0], array[3])
        #         board[i][j] = new_piece
                    
        
        #print_board(board, R, C)
        
        solve(board, pieces, R, C)

        # print(index_to_array)
        # start = time()
        # if resolve(board, pieces_subset):
        #     outln(time() - start)
        #     print_board(board)
        # else:
        #     outln(time() - start)
        #     outln("impossible puzzle!")
