#include <iostream>
#include <vector>

#include "board.h"
#include "colors.h"
using namespace std;

Board::Board() {
    grid.resize(8, vector<Piece>(8, Piece(PieceType::EMPTY)));
}

void Board::initialPosition() {
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                grid[row][col] = Piece(PieceType::BLACK);
                grid[7 - row][col] = Piece(PieceType::WHITE);
            }
        }
    }
}

void Board::printBoard() {
    cout << "   A  B  C  D  E  F  G  H\n";

    for (int row = 0; row < 8; row++) {
        cout << (row + 1 < 10 ? " " : "") << row + 1 << "  ";
        for (int col = 0; col < 8; col++) {
            Piece &p = grid[row][col];

            if (p.getType() == PieceType::EMPTY) {
                cout << ".  ";
            } else if (p.getType() == PieceType::WHITE) {
                cout << WHITE_COLOR << "w" << RESET << "  ";
            } else if (p.getType() == PieceType::BLACK) {
                cout << RED_COLOR << "b" << RESET << "  ";
            } else if (p.getType() == PieceType::WHITE_KING) {
                cout << "♔" << RESET << "  ";
            } else if (p.getType() == PieceType::BLACK_KING) {
                cout << "♚" << RESET << "  ";
            }
        }
        cout << "\n";
    }
}

void Board::movePiece(int from_row, int from_col, int to_row, int to_col) {
    // Обычный ход
    Piece &moving_piece = grid[from_row][from_col];
    grid[to_row][to_col] = moving_piece;
    grid[from_row][from_col].reset();

    // Превращаем клетку в дамку//
    if (moving_piece.isWhite() && to_row == 7)
        grid[to_row][to_col].makeKing();
    else if (moving_piece.isBlack() && to_row == 0)
        grid[to_row][to_col].makeKing();

    // Если съедаем вражескую шашку
    if (isCaptureMove(from_row, from_col, to_row, to_col)) {

        if (!moving_piece.isKing()) {
            int mid_row = (from_row + to_row) / 2;
            int mid_col = (from_col + to_col) / 2;
            grid[mid_row][mid_col].reset();
        } else {
            int row_step = (to_row > from_row) ? 1 : -1;
            int col_step = (to_col > from_col) ? 1 : -1;
            int current_row = from_row + row_step;
            int current_col = from_col + col_step;

            while (current_row != to_row && current_col != to_col) {
                Piece &current = grid[current_row][current_col];
                if ((moving_piece.isWhite() && current.isBlack()) ||
                    (moving_piece.isBlack() && current.isWhite())) {
                    current.reset();
                    break;
                }
                current_row += row_step;
                current_col += col_step;
            }
        }
    }
}
bool Board::isCaptureMove(int from_row, int from_col, int to_row, int to_col) {
    Piece &moving_piece = grid[from_row][from_col];
    Piece &to_piece = grid[to_row][to_col];

    if (to_piece.getType() != PieceType::EMPTY) return false;

    if (!moving_piece.isKing()) {
        if (abs(from_row - to_row) == 2 && abs(from_col - to_col) == 2) {
            int mid_row = (from_row + to_row) / 2;
            int mid_col = (from_col + to_col) / 2;
            Piece &middle = grid[mid_row][mid_col];

            if (middle.getType() != PieceType::EMPTY &&
                middle.getType() != moving_piece.getType()) {
                return true;
            }
        }
    } else if (abs(from_row - to_row) == abs(from_col - to_col) && abs(from_row - to_row) > 1) {

        int row_step = (to_row > from_row) ? 1 : -1;
        int col_step = (to_col > from_col) ? 1 : -1;
        int current_row = row_step + from_row;
        int current_col = col_step + from_col;
        int enemy_count = 0;

        while (current_row != to_row && current_col != to_col) {
            Piece &currentPiece = grid[current_row][current_col];

            if ((currentPiece.isBlack() && moving_piece.isWhite()) ||
                (currentPiece.isWhite() && moving_piece.isBlack())) {
                enemy_count++;
                if (enemy_count > 1) return false;
            } else
                return false;

            current_col += col_step;
            current_row += row_step;
        }
        return enemy_count == 1;
    }
    return false;
}

bool Board::isValidMove(int from_row, int from_col, int to_row, int to_col) {
    Piece &moving_piece = grid[from_row][from_col];
    Piece &to_piece = grid[to_row][to_col];

    if (from_row < 0 || from_row >= 8 || from_col < 0 || from_col >= 8 || to_row < 0 ||
        to_row >= 8 || to_col < 0 || to_col >= 8)
        return false;

    if (to_piece.getType() != PieceType::EMPTY) return false;

    if (moving_piece.isWhite() && (from_row - 1 == to_row) &&
        ((from_col - 1 == to_col) || (from_col + 1 == to_col)))
        return true;
    else if (moving_piece.isBlack() && (from_row + 1 == to_row) &&
             ((from_col - 1 == to_col) || (from_col + 1 == to_col)))
        return true;

    if (isCaptureMove(from_row, from_col, to_row, to_col)) return true;

    return false;
}

bool Board::canAttackAgain(int row, int col) {
    Piece &piece = grid[row][col];
    int possible_move[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    if (piece.getType() == PieceType::EMPTY) return false;
    for (int type_move = 0; type_move < 4; type_move++) {
        int d_row = possible_move[type_move][0];
        int d_col = possible_move[type_move][1];

        if (!piece.isKing()) {
            int new_row = row + d_row * 2;
            int new_col = col + d_col * 2;

            if (new_row < 0 || new_row >= 8 || new_col < 0 || new_col >= 8) continue;

            if (isCaptureMove(row, col, new_row, new_col)) return true;
        } else {
            int current_row = row + d_row;
            int current_col = col + d_col;

            while (current_row >= 0 && current_row < 8 && current_col >= 0 && current_col < 8) {
                if (isCaptureMove(row, col, current_row, current_col)) return true;
                current_row += d_row;
                current_col += d_col;
            }
        }
    }
    return false;
}
