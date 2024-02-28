from typing import List, Union, Tuple
from io import TextIOWrapper
import numpy as np
from math import inf
import time as t


class TicTacToeBoard:
    def __init__(self, board=None, turn=1):
        if board is None:
            self.board = np.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]])
        else:
            self.board = board
        self.turn = turn

    def make_move(self, x: int, y: int):
        new_board = np.array(self.board, dtype=int)
        new_board[x][y] = self.turn
        return TicTacToeBoard(new_board, 1 if self.turn == 2 else 2)

    def printToFile(self, file: TextIOWrapper):
        file.write(str(self) + '\n')

    def __str__(self):
        s = f'{"X" if self.turn == 1 else "O"}'
        for row in self.board:
            s += '\n'
            for cell in row:
                s += f'{"X" if cell == 1 else "O" if cell == 2 else " "}'
        return s

    def __repr__(self):
        return self.__str__()

    def getChildren(self) -> List['TicTacToeBoard']:
        moves = []
        for i in range(3):
            for j in range(3):
                if self.board[i][j] == 0:
                    moves.append(self.make_move(i, j))
        return moves

    def isFinal(self) -> int:
        # test if board is full
        if not 0 in self.board:
            return 0

        # test if there is a winner
        g1 = np.array([1, 1, 1])
        g2 = np.array([2, 2, 2])
        for i in range(3):
            if np.any(self.board[i] == g1) or np.any(self.board[:, i] == g1):
                return 1
            if np.any(self.board[i] == g2) or np.any(self.board[:, i] == g2):
                return 2

        if np.any(self.board.diagonal() == g1) or np.any(np.fliplr(self.board).diagonal() == g1):
            return 1
        if np.any(self.board.diagonal() == g2) or np.any(np.fliplr(self.board).diagonal() == g2):
            return 2

        return -1

    def __eq__(self, other: 'TicTacToeBoard'):
        return self.board == other.board and self.turn == other.turn

    def __hash__(self):
        return hash(str(self))


boards: List[TicTacToeBoard] = []
with open("dataset.txt", "r") as dataset:
    for line in dataset.readlines():
        board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        turn = 1 if line[0] == 'X' else 2
        for i in range(3):
            for j in range(3):
                if line[1 + 3 * i + j] == 'X':
                    board[i][j] = 1
                elif line[1 + 3 * i + j] == 'O':
                    board[i][j] = 2
        boards.append(TicTacToeBoard(board, turn))


def minimax(board: TicTacToeBoard, playerCurrent: int, turn: int) -> Tuple[int, TicTacToeBoard]:
    if v := board.isFinal() != -1:
        return (1 if v == turn else -1 if v != 0 else 0, board)

    if playerCurrent == turn:
        m = (-inf, None)
        for children in board.getChildren():
            rslt = minimax(children, playerCurrent, turn)
            if rslt[0] > m[0]:
                m = (rslt[0], children)
        return m
    else:
        m = (inf, None)
        for children in board.getChildren():
            rslt = minimax(children, playerCurrent, turn)
            if rslt[0] < m[0]:
                m = (rslt[0], children)
        return m


print(boards[0], "\n")
print(minimax(boards[0], boards[0].turn, 1)[1])

start = t.time()
minimax(TicTacToeBoard(), 1, 1)
end = t.time()

print(end - start)
