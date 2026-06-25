#ifndef AI_H
#define AI_H

#include "board.h"

// AI handles computer move calculation
// Easy  -> picks a random empty cell
// Hard  -> uses minimax (never loses)

class AI {
private:
    char aiMark;      // the mark AI is playing as
    char humanMark;   // the opponent's mark
    int  difficulty;  // 1 = easy, 2 = hard

    // minimax internals
    int  minimax(Board &board, bool isMaximizing);
    int  evaluate(Board &board);    // +10 AI wins, -10 human wins, 0 otherwise

public:
    AI(char aiMark, char humanMark, int difficulty);
    void makeMove(Board &board);    // decides and places the move
};

#endif
