#include <iostream>
#include <vector>

#include "board.h"
#include "colors.h"
using namespace std;
static constexpr int POSSIBLE_MOVE[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

Board::Board() {
    grid.resize(8, vector<Piece>(8, Piece(PieceType::EMPTY)));
}

Piece &Board::getPiece(int row, int col) {
    return grid[row][col];
}

void Board::initialPosition() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                grid[row][col] = Piece(PieceType::BLACK);
            } else
                grid[7 - row][col] = Piece(PieceType::WHITE);
        }
    }
}

void Board::printBoard() {
    cout << "    A  B  C  D  E  F  G  H\n";

    for (int row = 0; row < 8; row++) {
        cout << (row + 1 < 10 ? " " : "") << row + 1 << "  ";
        for (int col = 0; col < 8; col++) {
            Piece &p = grid[row][col];
            string cellColor = ((row + col) % 2 == 0) ? BG_WHITE : BG_BLACK;

            string pieceSymbol;
            if (p.getType() == PieceType::EMPTY) {
                pieceSymbol = " ";
            } else if (p.getType() == PieceType::WHITE) {
                pieceSymbol = YELLOW_COLOR "w" RESET;
            } else if (p.getType() == PieceType::BLACK) {
                pieceSymbol = RED_COLOR "b" RESET;
            } else if (p.getType() == PieceType::WHITE_KING) {
                pieceSymbol = YELLOW_COLOR "♔" RESET;
            } else if (p.getType() == PieceType::BLACK_KING) {
                pieceSymbol = RED_COLOR "♚" RESET;
            }

            cout << cellColor << " " << pieceSymbol << " " << RESET;
        }
        cout << "\n";
    }
}

void Board::movePiece(int from_row, int from_col, int to_row, int to_col) {
    // Обычный ход
    Piece &moving_piece = grid[from_row][from_col];
    Piece temp_piece = moving_piece;
    bool is_capture = isCaptureMove(from_row, from_col, to_row, to_col);
    grid[to_row][to_col] = moving_piece;
    grid[from_row][from_col].reset();

    // Превращаем клетку в дамку//
    if (temp_piece.isWhite() && to_row == 0)
        grid[to_row][to_col].makeKing();
    else if (temp_piece.isBlack() && to_row == 7)
        grid[to_row][to_col].makeKing();

    // Если съедаем вражескую шашку
    if (is_capture) {
        if (!temp_piece.isKing()) {
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
                if ((temp_piece.isWhite() && current.isBlack()) ||
                    (temp_piece.isBlack() && current.isWhite())) {
                    current.reset();
                    break;
                }
                current_row += row_step;
                current_col += col_step;
            }
        }
    }
}

bool Board::isInsideBoard(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::checkPath(int from_row, int from_col, int to_row, int to_col, const Piece &moving_piece,
                      int max_enemies) const {
    if (abs(from_row - to_row) != abs(from_col - to_col)) return false;

    int row_step = (to_row > from_row) ? 1 : -1;
    int col_step = (to_col > from_col) ? 1 : -1;
    int r = from_row + row_step;
    int c = from_col + col_step;
    int enemy_count = 0;

    while (r != to_row && c != to_col) {
        const Piece &current = grid[r][c];

        if ((moving_piece.isWhite() && current.isBlack()) ||
            (moving_piece.isBlack() && current.isWhite())) {
            enemy_count++;
            if (enemy_count > max_enemies) return false;
        } else if (current.getType() != PieceType::EMPTY) {
            return false;
        }
        r += row_step;
        c += col_step;
    }
    return enemy_count == max_enemies;
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
    } else if (abs(from_row - to_row) == abs(from_col - to_col) && abs(from_row - to_row) > 1)
        return checkPath(from_row, from_col, to_row, to_col, moving_piece, 1);

    return false;
}

bool Board::isValidMove(int from_row, int from_col, int to_row, int to_col) {
    Piece &moving_piece = grid[from_row][from_col];
    Piece &to_piece = grid[to_row][to_col];

    if (!isInsideBoard(to_row, to_col)) {
        return false;
    }

    if (to_piece.getType() != PieceType::EMPTY) {
        return false;
    }

    if (isCaptureMove(from_row, from_col, to_row, to_col)) return true;

    if (hasMandatoryCapture(moving_piece.getType())) {
        return false;
    }

    if (!moving_piece.isKing()) {

        if (moving_piece.isWhite() && (from_row - 1 == to_row) &&
            ((from_col - 1 == to_col) || (from_col + 1 == to_col)))
            return true;

        else if (moving_piece.isBlack() && (from_row + 1 == to_row) &&
                 ((from_col - 1 == to_col) || (from_col + 1 == to_col)))
            return true;

    } else if (abs(from_row - to_row) == abs(from_col - to_col))
        return checkPath(from_row, from_col, to_row, to_col, moving_piece, 0);
    return false;
}

bool Board::hasDiagonalCapture(int row, int col, int d_row, int d_col) {
    int current_row = row + d_row;
    int current_col = col + d_col;
    while (isInsideBoard(current_row, current_col)) {
        if (isCaptureMove(row, col, current_row, current_col)) return true;
        current_row += d_row;
        current_col += d_col;
    }
    return false;
}

bool Board::canAttackAgain(int row, int col) {
    Piece &piece = grid[row][col];

    if (piece.getType() == PieceType::EMPTY) return false;

    for (int type_move = 0; type_move < 4; type_move++) {
        int d_row = POSSIBLE_MOVE[type_move][0];
        int d_col = POSSIBLE_MOVE[type_move][1];

        if (!piece.isKing()) {
            int new_row = row + d_row * 2;
            int new_col = col + d_col * 2;

            if (!isInsideBoard(new_row, new_col)) continue;
            if (isCaptureMove(row, col, new_row, new_col)) return true;
        } else if (hasDiagonalCapture(row, col, d_row, d_col))
            return true;
    }
    return false;
}

bool Board::hasMandatoryCapture(PieceType player) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece &piece = grid[row][col];
            if (piece.getType() == PieceType::EMPTY) continue;

            if ((piece.isWhite() && player == PieceType::BLACK) ||
                (piece.isBlack() && player == PieceType::WHITE))
                continue;

            for (int type_move = 0; type_move < 4; type_move++) {
                int d_row = POSSIBLE_MOVE[type_move][0];
                int d_col = POSSIBLE_MOVE[type_move][1];

                if (!piece.isKing()) {
                    int new_row = row + d_row * 2;
                    int new_col = col + d_col * 2;

                    if (!isInsideBoard(new_row, new_col)) continue;

                    if (isCaptureMove(row, col, new_row, new_col)) return true;
                } else if (hasDiagonalCapture(row, col, d_row, d_col))
                    return true;
            }
        }
    }
    return false;
}

bool Board::hasAnyMoves(PieceType player) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece &piece = grid[row][col];
            if (piece.getType() == PieceType::EMPTY) continue;

            if ((piece.isWhite() && player == PieceType::BLACK) ||
                (piece.isBlack() && player == PieceType::WHITE))
                continue;

            for (int i = 0; i < 4; i++) {
                int d_row = POSSIBLE_MOVE[i][0];
                int d_col = POSSIBLE_MOVE[i][1];

                if (!piece.isKing()) {
                    int new_row = row + d_row;
                    int new_col = col + d_col;
                    int cap_row = row + d_row * 2;
                    int cap_col = col + d_col * 2;

                    if ((isInsideBoard(new_row, new_col) &&
                         isValidMove(row, col, new_row, new_col)) ||
                        isValidMove(row, col, cap_row, cap_col))

                        return true;

                } else if (hasDiagonalCapture(row, col, d_row, d_col))
                    return true;
            }
        }
    }
    return false;
}

void Board::clear() {
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col) grid[row][col] = Piece();
}
