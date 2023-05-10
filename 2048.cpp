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
    void print() const;
    //true if success, false if board is full and cannot generate
    void generate();
    void combine(char move);
    bool full() const;
    bool gameOver() const;
    bool isEqual(const Board& other) const;
    Board& operator=(const Board& other);
};

Board::Board() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = 0;
        }
    }
}

void Board::print() const {
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

void Board::generate() {
    /* initialize random seed: */
    srand(time(NULL));

    /* generate secret number between 0 and 3: */

    int row = rand() % 4;
    int col = rand() % 4;
    while (board[row][col] != 0) {
        if (full()) {
            return;
        }
        row = rand() % 4;
        col = rand() % 4;
    }

    //1 to 10, if 1, generate 4, else generate 2
    //simulates 90% 2 and 10% 4
    int num = rand() % 10 + 1;
    if (num == 1) {
        board[row][col] = 4;
    }
    else {
        board[row][col] = 2;
    }
    
    
    //DEBUG
    //cout << endl << "DEBUG: Generated " << num << " at (" << col + 1 << ", " << BOARD_SIZE - row << ")" << endl;
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
        for (int i = 0; i < BOARD_SIZE - 1; ++i) {
            if (combinedRow[i] == combinedRow[i + 1]) {
                combinedRow[i] *= 2;
                combinedRow[i + 1] = 0;
            }
        }
        index = 0;
        int actualCombinedRow[4] = { 0,0,0,0 };
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (combinedRow[col] != 0) {
                actualCombinedRow[index++] = combinedRow[col];
            }
        }

        // Copy combined row back to the original row
        for (int i = 0; i < BOARD_SIZE; ++i) {
            board[row][i] = actualCombinedRow[i];
        }
    }
}

bool Board::full() const {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool Board::gameOver() const {
    if (full()) {
        int boardIndex = BOARD_SIZE - 1;
        //row 0,1,2; col 0,1,2
        for (int row = 0; row < boardIndex; row++) {
            for (int col = 0; col < boardIndex; col++) {
                if (board[row][col] == board[row + 1][col] ||
                    board[row][col] == board[row][col + 1]) {
                    return false;
                }
            }
        }

        //last col and last row
        
        for (int i = 0; i < boardIndex; i++) {
            if (board[boardIndex][i] == board[boardIndex][i + 1] ||
                board[i][boardIndex] == board[i + 1][boardIndex]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

Board& Board::operator=(const Board& other) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = other.board[row][col];
        }
    }
    return *this;
}

bool Board::isEqual(const Board& other) const {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] != other.board[row][col]) {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Board& lhs, const Board& rhs) {
    return !lhs.isEqual(rhs);
}

int main() {
    Board board;
    board.generate();
    while (!board.gameOver()) {
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
        
        //if new board == old board, dont generate
        Board tempBoard = board;
        board.combine(move);
        if (tempBoard != board) {
            board.generate();
        }
    }
    board.print();
    cout << "Game over";
    return 0;
}