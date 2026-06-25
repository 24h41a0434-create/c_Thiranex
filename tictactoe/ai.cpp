#include "ai.h"
#include <iostream>
#include <cstdlib>   // rand()
#include <ctime>     // time() for seeding
#include <climits>   // INT_MIN, INT_MAX
using namespace std;

AI::AI(char am, char hm, int diff) {
    aiMark    = am;
    humanMark = hm;
    difficulty = diff;
    srand((unsigned int)time(0));   // seed random once
}

// scores the current board from AI's perspective
int AI::evaluate(Board &board) {
    if (board.checkWin(aiMark))    return +10;
    if (board.checkWin(humanMark)) return -10;
    return 0;
}

// minimax: tries every possible move recursively
// isMaximizing = true  -> AI's turn (wants highest score)
// isMaximizing = false -> human's turn (wants lowest score)
int AI::minimax(Board &board, bool isMaximizing) {
    int score = evaluate(board);

    // terminal states: win or lose
    if (score == 10 || score == -10)
        return score;

    // draw
    if (board.isFull())
        return 0;

    if (isMaximizing) {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board.isCellEmpty(i, j)) {
                    board.placeMark(i, j, aiMark);
                    int val = minimax(board, false);
                    board.undoMark(i, j);   // undo the move
                    if (val > best) best = val;
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board.isCellEmpty(i, j)) {
                    board.placeMark(i, j, humanMark);
                    int val = minimax(board, true);
                    board.undoMark(i, j);   // undo
                    if (val < best) best = val;
                }
            }
        }
        return best;
    }
}

void AI::makeMove(Board &board) {
    if (difficulty == 1) {
        // Easy: just pick a random empty cell
        int row, col;
        do {
            row = rand() % 3;
            col = rand() % 3;
        } while (!board.isCellEmpty(row, col));

        board.placeMark(row, col, aiMark);
        cout << "AI plays at row " << (row+1) << ", col " << (col+1) << "\n";

    } else {
        // Hard: minimax finds the best possible move
        int bestVal = INT_MIN;
        int bestRow = -1, bestCol = -1;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board.isCellEmpty(i, j)) {
                    board.placeMark(i, j, aiMark);
                    int moveVal = minimax(board, false);
                    board.undoMark(i, j);   // undo

                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }

        board.placeMark(bestRow, bestCol, aiMark);
        cout << "AI plays at row " << (bestRow+1) << ", col " << (bestCol+1) << "\n";
    }
}
