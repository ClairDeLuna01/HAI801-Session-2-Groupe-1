from typing import List, Union, Tuple
from io import TextIOWrapper
# import numpy as np
from math import inf
import time as t
from functools import cache


class TicTacToeBoard:
    def __init__(self, board: List[List[int]] = None, turn=1):
        if board is None:
            self.board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        else:
            self.board = board
        self.turn = turn

    def make_move(self, x: int, y: int):
        new_board = [row.copy() for row in self.board]
        new_board[x][y] = self.turn
        return TicTacToeBoard(new_board, 1 if self.turn == 2 else 2)

    def printToFile(self, file: TextIOWrapper):
        file.write(str(self) + '\n')

    def __str__(self):
        s = f'turn: {"X" if self.turn == 1 else "O"}'
        for row in self.board:
            s += '\n'
            for cell in row:
                s += f'{"X" if cell == 1 else "O" if cell == 2 else " "}'
        return s

    def __repr__(self):
        return self.__str__()

    @cache
    def getChildren(self) -> List['TicTacToeBoard']:
        moves = []
        for i in range(3):
            for j in range(3):
                if self.board[i][j] == 0:
                    moves.append(self.make_move(i, j))
        return moves

    @cache
    def isFinal(self) -> int:
        # test if there is a winner
        for i in range(3):
            if self.board[i][0] == self.board[i][1] == self.board[i][2] != 0:
                return self.board[i][0]
            if self.board[0][i] == self.board[1][i] == self.board[2][i] != 0:
                return self.board[0][i]
        if self.board[0][0] == self.board[1][1] == self.board[2][2] != 0:
            return self.board[0][0]
        if self.board[0][2] == self.board[1][1] == self.board[2][0] != 0:
            return self.board[0][2]

        # test if board is full
        found_zero = False
        for row in self.board:
            for cell in row:
                if cell == 0:
                    found_zero = True
                    break
            if found_zero:
                break
        if not found_zero:
            return 0

        return -1

    def __eq__(self, other: 'TicTacToeBoard'):
        return self.board == other.board and self.turn == other.turn

    def __hash__(self) -> int:
        t = tuple([tuple(row) for row in self.board])
        return hash(t)


boards: List[TicTacToeBoard] = []
with open("dataset.txt", "r") as dataset:
    lines = dataset.readlines()
    for line in lines:
        board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        turn = 1 if line[0] == 'X' else 2
        for i in range(3):
            for j in range(3):
                if line[1 + 3 * i + j] == 'X':
                    board[i][j] = 1
                elif line[1 + 3 * i + j] == 'O':
                    board[i][j] = 2
        boards.append(TicTacToeBoard(board, turn))


@cache
def minimax(board: TicTacToeBoard, playerCurrent: int, turn: int, depth=0) -> Tuple[int, List[int]]:
    if (v := board.isFinal()) != -1:
        board.isFinal()
        return (10 - depth if v == turn else (-1 if v != 0 else 0), [])

    if playerCurrent == turn:
        m = (-99999, None)
        for i, child in enumerate(board.getChildren()):
            rslt = minimax(child, 1 if playerCurrent ==
                           2 else 2, turn, depth + 1)
            if rslt[0] > m[0]:
                m = (rslt[0], [i] + rslt[1])
        return m
    else:
        m = (99999, None)
        for i, child in enumerate(board.getChildren()):
            rslt = minimax(child, 1 if playerCurrent ==
                           2 else 2, turn, depth + 1)
            if rslt[0] < m[0]:
                m = (rslt[0], [i] + rslt[1])
        return m


# N = 1
# print(boards[N])
# print()
# x = minimax(boards[N], boards[N].turn, boards[N].turn)
# print(x[0])
# print("\n====\n")
# b = boards[N]
# for i in x[1]:
#     b = b.getChildren()[i]
#     print(b)
#     print("\n====\n")


start = t.time()
v = 0
for i, board in enumerate(boards[:]):
    x = minimax(board, board.turn, board.turn)[0]
    v += x
    # print(f"{i}:", x)
end = t.time()
print(end - start)
print(v)
