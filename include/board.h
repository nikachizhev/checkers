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
    piece &getPiece(int row, int col);
    void movePiece(int from_row, int from_col, int to_row, int to_col);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col);
    bool canAttackAgain(int row, int col);
};
#endif