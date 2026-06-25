#include "game.h"
#include <iostream>
#include <limits>
using namespace std;

Game::Game() {
    p1       = nullptr;
    p2       = nullptr;
    aiPlayer = nullptr;
    vsAI     = false;
    currentTurn = 0;

    cout << "\n=== Welcome to Tic Tac Toe ===\n\n";
    setupPlayers();
}

Game::~Game() {
    delete p1;
    delete p2;
    if (aiPlayer) delete aiPlayer;
}

void Game::setupPlayers() {
    // pick game mode
    int mode = 0;
    cout << "Select mode:\n";
    cout << "  1. Player vs Player\n";
    cout << "  2. Player vs AI\n";
    cout << "Choice: ";
    cin >> mode;

    string name1;
    cout << "\nEnter your name (Player 1, X): ";
    cin >> name1;
    p1 = new Player(name1, 'X');

    if (mode == 2) {
        vsAI = true;

        int diff = 0;
        cout << "\nSelect AI difficulty:\n";
        cout << "  1. Easy   (random moves)\n";
        cout << "  2. Hard   (unbeatable)\n";
        cout << "Choice: ";
        cin >> diff;
        if (diff != 1 && diff != 2) diff = 1;  // default to easy

        aiPlayer = new AI('O', 'X', diff);
        p2 = new Player("Computer", 'O');

        string diffLabel = (diff == 1) ? "Easy" : "Hard";
        cout << "\nOK! You're playing against Computer (" << diffLabel << ").\n";
    } else {
        vsAI = false;
        string name2;
        cout << "Enter name for Player 2 (O): ";
        cin >> name2;
        p2 = new Player(name2, 'O');
    }
}

Player* Game::getCurrentPlayer() {
    return (currentTurn == 0) ? p1 : p2;
}

void Game::switchTurn() {
    currentTurn = 1 - currentTurn;
}

void Game::getMove(int &row, int &col) {
    while (true) {
        cout << "Enter row (1-3) and column (1-3): ";
        cin >> row >> col;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter numbers only.\n";
            continue;
        }

        row--;
        col--;

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            cout << "Out of range! Row and column must be between 1 and 3.\n";
            continue;
        }

        break;
    }
}

void Game::start() {
    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {
        board.reset();
        currentTurn = 0;
        bool gameOver = false;

        cout << "\n--- New Game ---\n";
        if (!vsAI)
            cout << "How to play: enter row then column (e.g. 1 2 = row 1, col 2)\n";

        while (!gameOver) {
            board.display();

            Player *current = getCurrentPlayer();

            // AI's turn
            if (vsAI && currentTurn == 1) {
                cout << "Computer is thinking...\n";
                aiPlayer->makeMove(board);
            }
            // Human's turn
            else {
                cout << current->getName() << "'s turn (" << current->getMark() << ")\n";

                int row, col;
                getMove(row, col);

                if (!board.placeMark(row, col, current->getMark())) {
                    cout << "That cell is already taken! Try again.\n";
                    continue;   // don't switch turns
                }
            }

            // check win/draw after every move
            if (board.checkWin(current->getMark())) {
                board.display();
                if (vsAI && currentTurn == 1)
                    cout << "*** Computer wins! Better luck next time. ***\n";
                else
                    cout << "*** " << current->getName() << " wins! Congratulations! ***\n";
                gameOver = true;
            }
            else if (board.isFull()) {
                board.display();
                cout << "It's a draw! Well played!\n";
                gameOver = true;
            }
            else {
                switchTurn();
            }
        }

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
    }

    cout << "\nThanks for playing! Goodbye.\n\n";
}
