#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

// Player holds a name and their symbol (X or O)
class Player {
private:
    string name;
    char mark;     // 'X' or 'O'

public:
    Player(string playerName, char playerMark);
    string getName();
    char getMark();
};

#endif
