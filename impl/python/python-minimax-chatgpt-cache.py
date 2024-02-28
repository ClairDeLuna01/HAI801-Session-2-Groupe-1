import time as t
from functools import lru_cache


def is_winner(board, player):
    win_conditions = [
        [board[0], board[1], board[2]],
        [board[3], board[4], board[5]],
        [board[6], board[7], board[8]],
        [board[0], board[3], board[6]],
        [board[1], board[4], board[7]],
        [board[2], board[5], board[8]],
        [board[0], board[4], board[8]],
        [board[2], board[4], board[6]],
    ]
    return [player, player, player] in win_conditions


def is_board_full(board):
    return ' ' not in board


def get_possible_moves(board):
    return [i for i, x in enumerate(board) if x == ' ']


# Modify the minimax signature to accept board as a tuple
@lru_cache(maxsize=None)
def minimax(board_tuple, depth, is_maximizing, player, opponent):
    board = list(board_tuple)  # Convert tuple back to list for manipulation
    if is_winner(board, player):
        return 10 - depth
    elif is_winner(board, opponent):
        return -1
    elif is_board_full(board):
        return 0

    if is_maximizing:
        best_score = -float('inf')
        for move in get_possible_moves(board):
            board[move] = player
            score = minimax(tuple(board), depth + 1, False,
                            player, opponent)  # Pass as tuple
            board[move] = ' '
            best_score = max(score, best_score)
        return best_score
    else:
        best_score = float('inf')
        for move in get_possible_moves(board):
            board[move] = opponent
            score = minimax(tuple(board), depth + 1, True,
                            player, opponent)  # Pass as tuple
            board[move] = ' '
            best_score = min(score, best_score)
        return best_score


def find_best_move(board, player):
    opponent = 'O' if player == 'X' else 'X'
    best_score = -float('inf')
    best_move = None
    for move in get_possible_moves(board):
        board[move] = player
        score = minimax(tuple(board), 0, False, player, opponent)
        board[move] = ' '
        if score > best_score:
            best_score = score
            best_move = move
    return best_move


# Reset the cache before using the function to ensure previous runs don't affect the results
minimax.cache_clear()

boards = []
turns = []
with open("dataset.txt", "r") as dataset:
    lines = dataset.readlines()
    for line in lines:
        board = [' ', ' ', ' ',
                 ' ', ' ', ' ',
                 ' ', ' ', ' ']
        turn = 'X' if line[0] == 'X' else 'O'
        for i in range(3):
            for j in range(3):
                if line[1 + 3 * i + j] == 'X':
                    board[3 * i + j] = 'X'
                elif line[1 + 3 * i + j] == 'O':
                    board[3 * i + j] = 'O'
        boards.append(board)
        turns.append(turn)

# X's turn to play
start = t.time()
v = 0
for i, board in enumerate(boards):
    v += find_best_move(board, turns[i])
end = t.time()
print("Time taken:", end - start)
print("Total:", v)
