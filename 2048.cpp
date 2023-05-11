#include <iostream>
#include <string>
#include <stdlib.h>     
#include <time.h>       

using namespace std;

class Board {
private:
    int boardSize;
    int* board;

    /*
    effect: helper function for combine()
    */
    void combineLeft();

    /*
    requires row and col > 0 && row and col < boardSize
    effect: returns the index based on row and col
        simulates board[row][col], since board is a 1d array
    */
    size_t index(int row, int col) const;

    /*
    effect: copies other.board and other.boardSize
    */
    void copy(const Board& other);
public:
    /*
    Board constructor
    requires: size is between 2 and 25
    modifies: board
    effect: initialize board to 0, set boardSize to size
    */
    Board(int size);

    /*
    modifies: cout
    effect: print the board in a pretty way
    */
    void print() const;

    /*
    modifies: board
    effect: generate a number based on the rules of 2048
        90% 2 and 10% 4 in a random square on the board
        if the chosen square already contains a number, find a new square to generate in
    */
    void generate();

    /*
    requires: move is 'a', 'w', 's', or 'd'
    modifies: board
    effect: combines the board based on the rules of 2048
        do not generate() if the board state does not change
    */
    void combine(char move);

    /*
    effect: returns true if board is full, false if board is not full
    */
    bool full() const;

    /*
    effect: returns true if the game is over based on the rules of 2048
        the game is over when there are no valid moves and the board is full
    */
    bool gameOver() const;

    /*
    effect: returns true if this->board contains the same numbers as other.board
    */
    bool isEqual(const Board& other) const;

    //big three
    Board(const Board& other);
    Board& operator=(const Board& rhs);
    ~Board();
};

Board::Board(int size) {
    boardSize = size;
    board = new int[size * size];
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[index(row, col)] = 0;
        }
    }
}

size_t Board::index(int row, int col) const {
    return row + boardSize * col;
}

void Board::print() const {
    for (int i = 0; i < boardSize; i++) {
        cout << "_______";
    }
    cout << endl;
    for (int row = 0; row < boardSize; row++) {

        for (int col = 0; col < boardSize; col++) {
            int num = board[index(row, col)];
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
        for (int i = 0; i < boardSize; i++) {
            cout << "|______";
        }
        cout << "|" << endl;
    }
}

void Board::generate() {
    /* initialize random seed: */
    srand(time(NULL));

    /* generate secret number between 0 and boardSize: */

    int row = rand() % boardSize;
    int col = rand() % boardSize;
    while (board[index(row, col)] != 0) {
        if (full()) {
            return;
        }
        row = rand() % boardSize;
        col = rand() % boardSize;
    }

    //1 to 10, if 1, generate 4, else generate 2
    //simulates 90% 2 and 10% 4
    int num = rand() % 10 + 1;
    if (num == 1) {
        num = 4;
        board[index(row, col)] = 4;
    }
    else {
        num = 2;
        board[index(row, col)] = 2;
    }

    //DEBUG
    //cout << endl << "DEBUG: Generated " << num << " at (" << col + 1 << ", " << boardSize - row << ")" << endl;
}

void Board::combine(char move) {
    Board transposedBoard(boardSize);
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            switch (move) {
            case 'w':
                transposedBoard.board[index(i, j)] = board[index(j, i)];
                break;
            case 'a':
                transposedBoard.board[index(i, j)] = board[index(i, j)];
                break;
            case 's':
                transposedBoard.board[index(i, j)] = board[index(boardSize - j - 1, boardSize - i - 1)];
                break;
            case 'd':
                transposedBoard.board[index(i, j)] = board[index(i, boardSize - j - 1)];
                break;
            }
        }
    }
    transposedBoard.combineLeft();
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            switch (move) {
            case 'w':
                board[index(i, j)] = transposedBoard.board[index(j, i)];
                break;
            case 'a':
                board[index(i, j)] = transposedBoard.board[index(i, j)];
                break;
            case 's':
                board[index(i, j)] = transposedBoard.board[index(boardSize - j - 1, boardSize - i - 1)];
                break;
            case 'd':
                board[index(i, j)] = transposedBoard.board[index(i, boardSize - j - 1)];
                break;
            }
        }
    }
}

void Board::combineLeft() {
    for (int row = 0; row < boardSize; row++) {

        int* combinedRow = new int[boardSize];
        for (int i = 0; i < boardSize; i++) {
            combinedRow[i] = 0;
        }

        int idx = 0;
        for (int col = 0; col < boardSize; col++) {
            if (board[index(row, col)] != 0) {
                combinedRow[idx++] = board[index(row, col)];
            }
        }
        // Combine adjacent numbers with the same value
        for (int i = 0; i < boardSize - 1; ++i) {
            if (combinedRow[i] == combinedRow[i + 1]) {
                combinedRow[i] *= 2;
                combinedRow[i + 1] = 0;
            }
        }
        idx = 0;

        int* actualCombinedRow = new int[boardSize];
        for (int i = 0; i < boardSize; i++) {
            actualCombinedRow[i] = 0;
        }

        for (int col = 0; col < boardSize; col++) {
            if (combinedRow[col] != 0) {
                actualCombinedRow[idx++] = combinedRow[col];
            }
        }

        // Copy combined row back to the original row
        for (int i = 0; i < boardSize; ++i) {
            board[index(row, i)] = actualCombinedRow[i];
        }

        delete[] combinedRow;
        delete[] actualCombinedRow;
    }
}

bool Board::full() const {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (board[index(row, col)] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool Board::gameOver() const {
    if (full()) {
        int boardIndex = boardSize - 1;
        //row 0,1,2; col 0,1,2
        for (int row = 0; row < boardIndex; row++) {
            for (int col = 0; col < boardIndex; col++) {
                if (board[index(row, col)] == board[index(row + 1, col)] ||
                    board[index(row, col)] == board[index(row, col + 1)]) {
                    return false;
                }
            }
        }

        //last col and last row

        for (int i = 0; i < boardIndex; i++) {
            if (board[index(boardIndex, i)] == board[index(boardIndex, i + 1)] ||
                board[index(i, boardIndex)] == board[index(i + 1, boardIndex)]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Board::isEqual(const Board& other) const {
    if (boardSize != other.boardSize) {
        return false;
    }
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (board[index(row, col)] != other.board[index(row, col)]) {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Board& lhs, const Board& rhs) {
    return !lhs.isEqual(rhs);
}

void Board::copy(const Board& other) {
    boardSize = other.boardSize;
    board = new int[boardSize * boardSize];
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            board[index(row, col)] = other.board[index(row, col)];
        }
    }
}

//copy ctor
Board::Board(const Board& other) {
    copy(other);
}

//assignment operator
Board& Board::operator=(const Board& rhs) {
    if (this == &rhs) {
        return *this;
    }
    delete[] board;
    copy(rhs);
    return *this;
}

//dtor
Board::~Board() {
    delete[] board;
}

int sizeInput() {
    int size;
    cout << "Input a size: ";
    cin >> size;
    while (size < 2 || size > 26) {
        cout << "ERROR: SIZE MUST BE GREATER THAN 2 AND LESS THAN 25" << endl;
        //clear the buffer
        if (cin.fail()) {
            cin.clear();
            string str;
            getline(cin, str);

        }
        cout << "Input a size: ";
        cin >> size;
    }
    return size;
}

int main() {
    int size = sizeInput();
    Board board(size);
    board.generate();
    while (!board.gameOver()) {
        system("cls");
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
    system("cls");
    board.print();
    cout << "Game over";
    return 0;
}