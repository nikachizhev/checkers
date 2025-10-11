#ifndef GAME_H
#define GAME_H
#include <string>

#include "board.h"

class Game {
  private:
    Board board;
    PieceType currentPlayer;
    bool gameOver;

  public:
    Game();
    void run();
    void switchPlayer();
    void getTurn();
    bool parseInput(std::string input, int &from_row, int &from_col, int &to_row, int &to_col);
    bool parseMove(int &from_row, int &from_col, int &to_row, int &to_col);
};
#endif