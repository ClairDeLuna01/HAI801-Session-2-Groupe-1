#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// N : Nobody wins/Nothing special happening
// T : Tie
// X : X wins
// O : O wins 
char finalStatePlayer(const std::string& stateGrid) {
    // Vertical
    for (int i{}; i < 3; ++i) {
        if (stateGrid[i] != ' ' && stateGrid[i] == stateGrid[i + 3] && stateGrid[i] == stateGrid[i + 6]) {
            return stateGrid[i];
        }
    }

    // Horizontal
    for (int i{}; i < 3; ++i) {
        if (stateGrid[3 * i] != ' ' && stateGrid[3 * i] == stateGrid[3 * i + 1] && stateGrid[3 * i] == stateGrid[3 * i + 2]) {
            return stateGrid[3 * i];
        }
    }

    // Diagonal
    if (stateGrid[4] != ' ' && (stateGrid[0] == stateGrid[4] && stateGrid[0] == stateGrid[8] || stateGrid[2] == stateGrid[4] && stateGrid[2] == stateGrid[6])) {
        return stateGrid[4];
    }

    for (int i{}; i < 9; ++i) {
        if (stateGrid[i] == ' ') {
            return 'N';
        }
    }

    return 'T';
}

std::vector<std::string> nextStates(const std::string& stateGrid, char charToPlace) {
    std::vector<std::string> nextStates;
    for (int i{}; i < 9; ++i) {
        if (stateGrid[i] == ' ') {
            std::string nextStateGrid{stateGrid};
            nextStateGrid[i] = charToPlace;
            nextStates.push_back(nextStateGrid);
        }
    }
    return nextStates;
}

int minmax(const std::string& grid, char player, int depth = 0) {
    char info{finalStatePlayer(grid)};

    if (info != 'N') {
        if (info == 'T') {
            return 0;
        }
        else if (info == 'X') {
            return 10 - depth;
        }
        else {
            return -1;
        }
    }
    else if (player == 'X') {

        int maxScore{std::numeric_limits<int>::min()};
        for (auto& a : nextStates(grid, 'X')) {
            maxScore = std::max(maxScore, minmax(a, 'O', depth + 1));
        }

        return maxScore;
    }
    else {
        int minScore{std::numeric_limits<int>::max()};

        for (auto& a : nextStates(grid, 'O')) {
            minScore = std::min(minScore, minmax(a, 'X', depth + 1));
        }

        return minScore;
    }
}

int main() {
    int x{};
    std::ifstream dataSet{"dataset.txt"};
    std::string line;

    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(dataSet, line)) {
        std::string grid{line.substr(1)};

        int xCount{};
        int oCount{};
        for (int i{}; i < 9; ++i) {
            if (grid[i] == 'X') {
                ++xCount;
            }
            else if (grid[i] == 'O') {
                ++oCount;
            }
        }

        char player;
        if (xCount > oCount) {
            player = 'O';
        }
        else if (xCount < oCount) {
            player = 'X';
        }
        else {
            player = line[0];
        }

        int mm{minmax(grid, player)};
        x += mm;
        std::cout << grid << " -> " << mm << '\n';
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << elapsedTime << " ms\n";
    std::cout << x << '\n';

    //minmax(" X O  XOX", 'O');
}
