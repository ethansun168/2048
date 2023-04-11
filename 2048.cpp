#include <iostream>
#include <string>
#include <stdlib.h>     
#include <time.h>       



using namespace std;

class Board {
private:
    static const int BOARD_SIZE = 4;
    int board[BOARD_SIZE][BOARD_SIZE];
    void combineLeft();
public:
    Board();
    void print();
    //true if success, false if board is full and cannot generate
    bool generate();
    void combine(char move);
    bool full();

};

Board::Board() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = 0;
        }
    }
}

void Board::print() {
    cout << "____________________________" << endl;
    for (int row = 0; row < BOARD_SIZE; row++) {

        for (int col = 0; col < BOARD_SIZE; col++) {
            int num = board[row][col];
            if (num == 0) {
                cout << "|     " << " ";
            }
            else if (num > 0 && num < 10) {
                cout << "|    " << num << " ";
            }
            else if (num >= 10 && num < 100) {
                cout << "|   " << num << " ";
            }
            else if (num >= 100 && num < 1000) {
                cout << "|  " << num << " ";
            }
            else {
                cout << "| " << num << " ";
            }
        }
        cout << "|" << endl;
        cout << "|______|______|______|______|" << endl;
    }
}

bool Board::generate() {
    /* initialize random seed: */
    srand(time(NULL));

    /* generate secret number between 0 and 3: */

    int row = rand() % 4;
    int col = rand() % 4;
    while (board[row][col] != 0) {
        if (full()) {
            return false;
        }
        row = rand() % 4;
        col = rand() % 4;
    }

    int num = 2 * (rand() % 2 + 1);
    board[row][col] = num;

    return true;
}

void Board::combine(char move) {
    if (move == 'w') {
        Board transposedBoard;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                transposedBoard.board[i][j] = board[j][i];
            }
        }
        transposedBoard.combineLeft();
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = transposedBoard.board[j][i];
            }
        }
    }
    else if (move == 'a') {
        combineLeft();
    }
    else if (move == 's') {
        Board transposedBoard;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                transposedBoard.board[i][j] = board[BOARD_SIZE - j - 1][BOARD_SIZE - i - 1];
            }
        }
        transposedBoard.combineLeft();
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = transposedBoard.board[BOARD_SIZE - j - 1][BOARD_SIZE - i - 1];
            }
        }
    }
    else {
        Board transposedBoard;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                transposedBoard.board[i][j] = board[i][BOARD_SIZE - j - 1];
            }
        }
        transposedBoard.combineLeft();
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = transposedBoard.board[i][BOARD_SIZE - j - 1];
            }
        }
    }
}

void Board::combineLeft() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        int combinedRow[4] = { 0,0,0,0 };
        int index = 0;
        for (int col = 0; col < BOARD_SIZE; col++) {

            if (board[row][col] != 0) {
                combinedRow[index++] = board[row][col];
            }
        }
        // Combine adjacent numbers with the same value
        for (int i = 0; i < 3; ++i) {
            if (combinedRow[i] == combinedRow[i + 1]) {
                combinedRow[i] *= 2;
                combinedRow[i + 1] = 0;
            }
        }


        // Copy combined row back to the original row
        for (int i = 0; i < 4; ++i) {
            board[row][i] = combinedRow[i];
        }

        index = 0;
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] != 0) {
                combinedRow[index++] = board[row][col];
            }
        }
        for (int i = 0; i < 4; ++i) {
            board[row][i] = combinedRow[i];
        }
    }
}

bool Board::full() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    Board board;

    while (board.generate()) {
        board.print();
        cout << "Move (w,a,s,d): \n";
        char move;
        cin >> move;
        while (move != 'a' && move != 'w' && move != 's' && move != 'd') {
            cout << "Invalid input! Enter something valid:\n";
            cin.clear();
            string temp;
            getline(cin, temp);
            cin >> move;
        }
        board.combine(move);
    }

    cout << "Game over";
    return 0;
}