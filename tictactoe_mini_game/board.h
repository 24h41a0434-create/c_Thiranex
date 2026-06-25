#ifndef BOARD_H
#define BOARD_H

// Board class handles the 3x3 grid
// stores cells as chars: ' ', 'X', or 'O'

class Board {
private:
    char grid[3][3];

public:
    Board();               // sets all cells to ' '
    void display();        // prints the board to console
    bool isCellEmpty(int row, int col);
    bool placeMark(int row, int col, char mark);
    void undoMark(int row, int col);   // used by minimax to undo a move
    bool checkWin(char mark);
    bool isFull();
    void reset();
};

#endif
