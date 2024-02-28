#include <iostream>
#include <vector>
#include <limits.h>
#include <chrono>

#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

// Function to evaluate the state of the board (win, lose, draw)
int evaluate(char b[3][3], char player)
{
    // Determine opponent based on the player
    char opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;

    // Checking for Rows for X or O victory.
    for (int row = 0; row < 3; row++)
    {
        if (b[row][0] == b[row][1] && b[row][1] == b[row][2])
        {
            if (b[row][0] == player)
                return +10;
            else if (b[row][0] == opponent)
                return -10;
        }
    }

    // Checking for Columns for X or O victory.
    for (int col = 0; col < 3; col++)
    {
        if (b[0][col] == b[1][col] && b[1][col] == b[2][col])
        {
            if (b[0][col] == player)
                return +10;
            else if (b[0][col] == opponent)
                return -10;
        }
    }

    // Checking for Diagonals for X or O victory.
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2])
    {
        if (b[0][0] == player)
            return +10;
        else if (b[0][0] == opponent)
            return -10;
    }

    if (b[0][2] == b[1][1] && b[1][1] == b[2][0])
    {
        if (b[0][2] == player)
            return +10;
        else if (b[0][2] == opponent)
            return -10;
    }

    // Else if none of them have won then return 0
    return 0;
}

// Function to check if there are moves remaining on the board
bool isMovesLeft(char board[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == EMPTY)
                return true;
    return false;
}

// The minimax function considers all the possible ways the game can go and returns the value of the board
int minimax(char board[3][3], int depth, bool isMax, char player)
{
    char opponent = (player == PLAYER_X) ? PLAYER_O : PLAYER_X;
    int score = evaluate(board, player);

    // If Maximizer has won the game return his/her evaluated score
    if (score == 10)
        return score - depth;

    // If Minimizer has won the game return his/her evaluated score
    if (score == -10)
        return -1;

    // If there are no more moves and no winner then it is a tie
    if (!isMovesLeft(board))
        return 0;

    // If this maximizer's move
    if (isMax)
    {
        int best = INT_MIN;

        // Traverse all cells
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                // Check if cell is empty
                if (board[i][j] == EMPTY)
                {
                    // Make the move
                    board[i][j] = player;

                    // Call minimax recursively and choose the maximum value
                    best = std::max(best, minimax(board, depth + 1, !isMax, player));

                    // Undo the move
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    }

    // If this minimizer's move
    else
    {
        int best = INT_MAX;

        // Traverse all cells
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                // Check if cell is empty
                if (board[i][j] == EMPTY)
                {
                    // Make the move
                    board[i][j] = opponent;

                    // Call minimax recursively and choose the minimum value
                    best = std::min(best, minimax(board, depth + 1, !isMax, player));

                    // Undo the move
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    }
}

// This will return the best possible move for the player
int findBestMove(char board[3][3], char player)
{
    int bestVal = (player == PLAYER_X) ? INT_MIN : INT_MAX;
    std::pair<int, int> bestMove = {-1, -1};
    bool isMax = (player == PLAYER_X) ? true : false;

    // Traverse all cells, evaluate minimax function for all empty cells. And return the cell with optimal value.
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // Check if cell is empty
            if (board[i][j] == EMPTY)
            {
                // Make the move
                board[i][j] = player;

                // Compute evaluation function for this move.
                int moveVal = minimax(board, 0, !isMax, player);

                // Undo the move
                board[i][j] = EMPTY;

                // If the value of the current move is more than the best value, then update best
                if ((isMax && moveVal > bestVal) || (!isMax && moveVal < bestVal))
                {
                    bestMove.first = i;
                    bestMove.second = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    // std::cout << "The value of the best Move for player " << player << " is : " << bestVal << "\n";
    return bestVal;
}

struct data
{
    char turn;
    char board[3][3];
    char stencil;
};

int main()
{
    FILE *fp = fopen("dataset.txt", "r");
    data d[4518];
    fread(d, sizeof(data), 4518, fp);

    int v = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 4518; i++)
    {
        v += findBestMove(d[i].board, d[i].turn);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    std::cout << v << std::endl;

    return 0;
}
