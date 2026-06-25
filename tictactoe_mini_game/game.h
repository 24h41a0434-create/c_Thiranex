#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include "ai.h"

// Game class ties everything together
// handles turns, input, win/draw detection, replay
// supports Player vs Player and Player vs AI modes
class Game {
private:
    Board board;
    Player *p1;
    Player *p2;
    AI    *aiPlayer;      // null if PvP mode
    bool   vsAI;          // true = playing against computer
    int    currentTurn;   // 0 = player1, 1 = player2/AI

    Player* getCurrentPlayer();
    void switchTurn();
    void getMove(int &row, int &col);
    void setupPlayers();   // asks mode, names, difficulty

public:
    Game();
    ~Game();
    void start();
};

#endif
