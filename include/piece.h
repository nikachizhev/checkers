#ifndef PIECE_H
#define PIECE_H

enum class PieceType { EMPTY, WHITE, BLACK, WHITE_KING, BLACK_KING };

class Piece {
  private:
    PieceType type;

  public:
    Piece(PieceType type = PieceType::EMPTY);
    bool isWhite() const;
    bool isBlack() const;
    bool isKing() const;
    void MakeKing();
};

#endif