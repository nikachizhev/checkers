#include <iostream>
#include <string>

#include "game.h"
using namespace std;

Game::Game() {
    board.initialPosition();
    currentPlayer = PieceType::WHITE;
    gameOver = false;
}

void Game::run() {
    while (!gameOver) {
        board.printBoard();
        if (currentPlayer == PieceType::WHITE)
            cout << "Сейчас ход белых" << endl;
        else
            cout << "Сейчас ход черных" << endl;

        if (!board.hasAnyMoves(currentPlayer)) {
            if (currentPlayer == PieceType::WHITE)
                cout << "Ход невозможен. Черные победили" << endl;
            else
                cout << "Ход невозможен. Белые победили" << endl;

            gameOver = true;
            break;
        }
        getTurn();
        switchPlayer();
    }
}

void Game::switchPlayer() {
    if (currentPlayer == PieceType::WHITE)
        currentPlayer = PieceType::BLACK;
    else
        currentPlayer = PieceType::WHITE;
}

bool Game::parseInput(string input, int &from_row, int &from_col, int &to_row, int &to_col) {
    if (input.length() < 5) {
        cout << "Вы ввели недостаточно данных, либо не поставили пробел" << endl;
        return false;
    }
    char from_col_char = toupper(input[0]);
    char from_row_char = input[1];
    char to_col_char = toupper(input[3]);
    char to_row_char = input[4];

    if (from_col_char < 'A' || from_col_char > 'H' || to_col_char < 'A' || to_col_char > 'H') {
        cout << "Для выбора столбца надо использовать буквы ABCDEFGH" << endl;
        return false;
    } else if (from_row_char < '1' || from_row_char > '8' || to_row_char < '1' ||
               to_row_char > '8') {
        cout << "Для выбора строки надо использовать цифры 12345678" << endl;
        return false;
    }
    from_col = from_col_char - 'A';
    from_row = from_row_char - '1';
    to_col = to_col_char - 'A';
    to_row = to_row_char - '1';
    return true;
}
bool Game::parseMove(int &from_row, int &from_col, int &to_row, int &to_col, int expected_row,
                     int expected_col) {
    bool valid = false;
    string input;
    while (!valid) {
        getline(cin, input);
        if (input == "1") {
            cout << "Игра завершена. Победили "
                 << (currentPlayer == PieceType::WHITE ? "чёрные" : "белые") << "!" << endl;
            exit(0);
        }
        if (!parseInput(input, from_row, from_col, to_row, to_col)) {
            cout << "Неверный формат ввода" << endl;
            continue;
        }
        if (expected_row != -1 && expected_col != -1) {
            if (from_row != expected_row || from_col != expected_col) {
                cout << "Вы должны продолжить атаку той же фигурой!" << endl;
                return false;
            }
        }

        Piece piece = board.getPiece(from_row, from_col);
        if ((currentPlayer == PieceType::WHITE && !piece.isWhite()) ||
            (currentPlayer == PieceType::BLACK && !piece.isBlack())) {
            cout << "Вы не можете ходить чужой фигурой!" << endl;
            continue;
        }
        if (board.hasMandatoryCapture(currentPlayer) &&
            !board.isCaptureMove(from_row, from_col, to_row, to_col)) {
            cout << "Вы должны сделать атакуюущий ход" << endl;
            continue;
        }
        if (!board.isValidMove(from_row, from_col, to_row, to_col)) {
            cout << "Недопустимый ход" << endl;
            continue;
        }

        valid = true;
    }
    return true;
}
void Game::getTurn() {
    int from_row, from_col, to_row, to_col;
    cout << "Введите ход в формате начальная позиция конечная позиция (например E3 f5)" << endl;
    bool wasCapture = false;
    if (parseMove(from_row, from_col, to_row, to_col)) {
        wasCapture = board.isCaptureMove(from_row, from_col, to_row, to_col);
        board.movePiece(from_row, from_col, to_row, to_col);

        int attacker_row = to_row;
        int attacker_col = to_col;

        while (wasCapture && board.canAttackAgain(attacker_row, attacker_col)) {
            board.printBoard();
            cout << "Вы можете сделать ещё один атакующий ход c " << char('A' + attacker_col)
                 << (1 + attacker_row) << endl;
            if (parseMove(from_row, from_col, to_row, to_col, attacker_row, attacker_col) &&
                board.isCaptureMove(from_row, from_col, to_row, to_col)) {
                board.movePiece(from_row, from_col, to_row, to_col);
                attacker_row = to_row;
                attacker_col = to_col;
                wasCapture = true;
            } else
                break;
        }
    }
}
