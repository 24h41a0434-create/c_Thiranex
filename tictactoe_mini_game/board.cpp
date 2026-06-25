#include "board.h"
#include <iostream>
using namespace std;

Board::Board() {
    // fill every cell with a space (empty)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = ' ';
}

void Board::display() {
    cout << "\n";
    cout << "     1   2   3\n";
    cout << "   +---+---+---+\n";

    for (int i = 0; i < 3; i++) {
        cout << " " << (i + 1) << " | ";
        for (int j = 0; j < 3; j++) {
            cout << grid[i][j];
            if (j < 2) cout << " | ";
        }
        cout << " |\n";
        cout << "   +---+---+---+\n";
    }
    cout << "\n";
}

bool Board::isCellEmpty(int row, int col) {
    return grid[row][col] == ' ';
}

bool Board::placeMark(int row, int col, char mark) {
    if (!isCellEmpty(row, col))
        return false;   // cell already taken

    grid[row][col] = mark;
    return true;
}

void Board::undoMark(int row, int col) {
    grid[row][col] = ' ';
}

bool Board::checkWin(char mark) {
    // check all 3 rows
    for (int i = 0; i < 3; i++) {
        if (grid[i][0] == mark && grid[i][1] == mark && grid[i][2] == mark)
            return true;
    }

    // check all 3 columns
    for (int j = 0; j < 3; j++) {
        if (grid[0][j] == mark && grid[1][j] == mark && grid[2][j] == mark)
            return true;
    }

    // check both diagonals
    if (grid[0][0] == mark && grid[1][1] == mark && grid[2][2] == mark)
        return true;
    if (grid[0][2] == mark && grid[1][1] == mark && grid[2][0] == mark)
        return true;

    return false;
}

bool Board::isFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i][j] == ' ')
                return false;
    return true;
}

void Board::reset() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = ' ';
}
