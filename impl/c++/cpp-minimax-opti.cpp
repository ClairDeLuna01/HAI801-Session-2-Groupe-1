#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <string>

#define STR_PLAYER0 'O'
#define STR_PLAYERX 'X'

struct playerID
{
    char value;
    bool is_playerO() const {return value == STR_PLAYER0;};
    bool is_playerX() const {return value == STR_PLAYERX;};;
    bool is_playerDraw() const {return value == 'd';};
    bool is_playerEMpty() const {return value == ' ';};
};

struct GameState
{
    // std::vector<playerID> grid;
    playerID currentPLayer;
    playerID grid[9];
    char stencil;

    playerID at(int i)
    {
        return grid[i];
    }

    playerID set(int i, playerID value)
    {
        grid[i] = value;
        return grid[i];
    }

    playerID est_final()
    {
        for(int i = 0; i < 3; i++)
        {

            if(!grid[i].is_playerEMpty() && grid[i].value == grid[i+3].value && grid[i].value == grid[i+6].value)
                return grid[i];      

            if(!grid[3*i].is_playerEMpty() && grid[3*i].value == grid[3*i+1].value && grid[3*i].value == grid[3*i+2].value)
                return grid[3*i];
        }

        if(!grid[0].is_playerEMpty() && grid[0].value == grid[4].value && grid[0].value == grid[8].value)
            return grid[0];

        if(!grid[2].is_playerEMpty() && grid[2].value == grid[4].value && grid[2].value == grid[6].value)
            return grid[2];

        for(int i = 0; i < 9; i++)
            if(grid[i].is_playerEMpty())
                return grid[i];
        
        return playerID{'d'};
    }

    int getChildren(GameState *children)
    {
        playerID nextPLayer;
        if(currentPLayer.is_playerO())
            nextPLayer = playerID({STR_PLAYERX});
        else
            nextPLayer = playerID({STR_PLAYER0});

        int id = 0;
        int idP = 0;

        
        for(auto &i : grid)
        {
            if(i.is_playerEMpty())
            {
                GameState newState = *this;
                // newState.set(id, currentPLayer);
                newState.grid[id] = currentPLayer;
                newState.currentPLayer = nextPLayer;
                children[idP++] = newState;
            }

            id++;
            
        }

        return idP;
    }
};

struct minimaxret
{
    GameState states[9];
    int maxid = 0;
    int val;

    void push_back(GameState &s)
    {
        states[maxid++] = s;
    }
};


void printPath2(GameState &r)
{
    std::cout << "======\n";
    // std::cout << r.val << "\n";
    for(int j = 0; j < 9; j++)
        if(j%3 == 2)
            std::cout << r.at(j).value << "\n";
        else
            std::cout << r.at(j).value << " ";
    std::cout << "======\n\n";
}

void printPath(minimaxret &r)
{
    // for(auto i : r.states)
    for(int id = 0; id < r.maxid; id++)
    {  
        GameState &i = r.states[id];
        std::cout << "======\n";
        for(int j = 0; j < 9; j++)
            if(j%3 == 2)
                std::cout << i.at(j).value << "\n";
            else
                std::cout << i.at(j).value << " ";
        std::cout << "======\n\n";
    }
}

minimaxret minimax(GameState &state, const playerID &playerToWIn, int depth = 0)
{
    minimaxret ret;
    
    playerID final = state.est_final();

    if(final.value != ' ')
    {
        ret.push_back(state);

        if(final.is_playerDraw())
            ret.val = 0;
        else
        if(playerToWIn.value == final.value)
            ret.val = 10 - depth;
        else
            ret.val = -1;

        return ret;
    }

    GameState children[9];
    int lim = state.getChildren(children);
    int sign = state.currentPLayer.value == playerToWIn.value ? -1 : 1;
    int m = 1e3*sign;

    for(int i = 0; i < lim; i++)
    {
        minimaxret m2 = minimax(children[i], playerToWIn, depth+1);
        if(m2.val*sign < m*sign)
        {
            m2.push_back(state);
            ret = m2;
            m = m2.val;
        }

        if(m >= 8) continue;
    }
   
    ret.val = m;
    
    return ret;
}



int main()
{
    std::ifstream file("dataset.txt", std::ios::in | std::ios::binary);

    std::vector<GameState> states;
    const int size = 4519;
    states.resize(size);
    file.read((char *)states.data(), sizeof(GameState)*size);

    int dummy = 0;

    for(int i = 0; i < size; i++)
    {
        minimaxret r = minimax(states[i], states[i].currentPLayer);
        dummy += r.val;
    }

    std::cout << dummy << "\n";

    file.close();

    return EXIT_SUCCESS;
}