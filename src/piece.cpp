#include "piece.h"

Piece::Piece(PieceType type) {
    this->type = type;
}

bool Piece::isWhite() const {
    return type == PieceType::WHITE || type == PieceType::WHITE_KING;
}

bool Piece::isBlack() const {
    return type == PieceType::BLACK || type == PieceType::BLACK_KING;
}

bool Piece::isKing() const {
    return type == PieceType::BLACK_KING || type == PieceType::WHITE_KING;
}

void Piece::MakeKing() {
    if (type == PieceType::WHITE)
        type = PieceType::WHITE_KING;
    else if (type == PieceType::BLACK)
        type = PieceType::BLACK_KING;
}