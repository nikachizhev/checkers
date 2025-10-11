#ifndef BOARD_H
#define BOARD_H
#include <vector>

#include "piece.h"

class Board {
  private:
    std::vector<std::vector<Piece>> grid;

  public:
    Board();
    void initialPosition();
    void printBoard();
    Piece &getPiece(int row, int col);
    void movePiece(int from_row, int from_col, int to_row, int to_col);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col);
    bool canAttackAgain(int row, int col);
    bool isCaptureMove(int from_row, int from_col, int to_row, int to_col);
    bool hasMandatoryCapture(PieceType player);
    bool hasAnyMoves(PieceType player);
    bool hasDiagonalCapture(int row, int col, int d_row, int d_coll);
    bool isInsideBoard(int row, int col) const;
};
#endif