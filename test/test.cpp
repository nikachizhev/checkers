#include <cassert>
#include <iostream>

#include "board.h"
#include "piece.h"
using namespace std;

void testMoveForward() {
    Board board;
    board.clear();
    board.getPiece(3, 2) = Piece(PieceType::WHITE);
    assert(board.isValidMove(3, 2, 2, 1));
}

void testdBackMove() {
    Board board;
    board.clear();
    board.getPiece(2, 1) = Piece(PieceType::BLACK);
    assert(!board.isValidMove(2, 1, 1, 0));
    cout << "testBackMove passed\n";
}

void testCaptureMove() {
    Board board;
    board.clear();
    board.getPiece(2, 2) = Piece(PieceType::WHITE); // C3
    board.getPiece(3, 3) = Piece(PieceType::BLACK); // D4
    assert(board.isCaptureMove(2, 2, 4, 4));        // C3 -> E5;
    cout << "testCaptureMove passed\n";
}

void testInvalidCaptureBlocked() {
    Board board;
    board.clear();
    board.getPiece(2, 2) = Piece(PieceType::WHITE); // C3
    board.getPiece(3, 3) = Piece(PieceType::BLACK); // D4
    board.getPiece(4, 4) = Piece(PieceType::WHITE); // E5 занята
    assert(!board.isCaptureMove(2, 2, 4, 4));
    cout << "testInvalidCaptureBlocked passed\n";
}

void testPromotionToKing() {
    Board board;
    board.clear();
    board.getPiece(1, 1) = Piece(PieceType::WHITE);
    board.movePiece(1, 1, 0, 0); // A2 -> A1
    assert(board.getPiece(0, 0).isKing());
    cout << "testPromotionToKing passed\n";
}

void testKingMoveAnyDirection() {
    Board board;
    board.clear();
    board.getPiece(4, 4) = Piece(PieceType::WHITE);
    board.getPiece(4, 4).makeKing();
    assert(board.isValidMove(4, 4, 2, 2)); // назад по диагонали
    assert(board.isValidMove(4, 4, 6, 6)); // вперёд по диагонали
    cout << "testKingMoveAnyDirection passed\n";
}

void testDoubleCapturePossible() {
    Board board;
    board.clear();
    board.getPiece(1, 1) = Piece(PieceType::WHITE); // B2
    board.getPiece(2, 2) = Piece(PieceType::BLACK); // C3
    board.getPiece(4, 4) = Piece(PieceType::BLACK); // E5

    assert(board.isCaptureMove(1, 1, 3, 3));
    board.movePiece(1, 1, 3, 3);

    assert(board.canAttackAgain(3, 3));
    assert(board.isCaptureMove(3, 3, 5, 5));

    cout << "testDoubleCapturePossible passed\n";
}

void testKingCapture() {
    Board board;
    board.clear();

    // Белая дамка на A1
    Piece whiteKing(PieceType::WHITE);
    whiteKing.makeKing();
    board.getPiece(0, 0) = whiteKing;

    // Чёрная шашка на C3
    board.getPiece(2, 2) = Piece(PieceType::BLACK);

    // E5 (4,4)
    assert(board.getPiece(4, 4).getType() == PieceType::EMPTY);

    assert(board.isCaptureMove(0, 0, 4, 4));
    assert(board.isValidMove(0, 0, 4, 4));

    board.movePiece(0, 0, 4, 4);

    assert(board.getPiece(2, 2).getType() == PieceType::EMPTY);

    // белая дамка на E5
    assert(board.getPiece(4, 4).isWhite());
    assert(board.getPiece(4, 4).isKing());

    cout << "testKingCapture passed\n";
}

void testKingCannotCaptureMultiplePieces() {
    Board board;
    board.clear();

    // Белая дамка на A1
    Piece whiteKing(PieceType::WHITE);
    whiteKing.makeKing();
    board.getPiece(0, 0) = whiteKing;

    // Две чёрные шашки на C3 (2,2) и D4 (3,3)
    board.getPiece(2, 2) = Piece(PieceType::BLACK);
    board.getPiece(3, 3) = Piece(PieceType::BLACK);

    assert(board.getPiece(4, 4).getType() == PieceType::EMPTY);

    // дамка НЕ может побить через две фигуры
    assert(!board.isCaptureMove(0, 0, 4, 4));
    assert(!board.isValidMove(0, 0, 4, 4));

    cout << "testKingCannotCaptureMultiplePieces passed\n";
}

void testBackwardCaptureByMan() {
    Board board;
    board.clear();

    // Чёрная шашка на C4 (3,2)
    board.getPiece(3, 2) = Piece(PieceType::BLACK);

    // Белая шашка на B3 (2,1)
    board.getPiece(2, 1) = Piece(PieceType::WHITE);

    assert(board.getPiece(1, 0).getType() == PieceType::EMPTY);

    // удар назад
    assert(board.isCaptureMove(3, 2, 1, 0));
    assert(board.isValidMove(3, 2, 1, 0));

    board.movePiece(3, 2, 1, 0);

    // белая шашка съедена
    assert(board.getPiece(2, 1).getType() == PieceType::EMPTY);

    // чёрная шашка на A2
    assert(board.getPiece(1, 0).isBlack());

    cout << "testBackwardCaptureByMan passed\n";
}
void testMandatoryCaptureBlocksNormalMove() {
    Board board;
    board.clear();

    // Белая шашка на C3
    board.getPiece(2, 2) = Piece(PieceType::WHITE);

    // Чёрная шашка на D4
    board.getPiece(3, 3) = Piece(PieceType::BLACK);

    // Целевая клетка для атаки — E5
    board.getPiece(4, 4) = Piece(PieceType::EMPTY);

    board.getPiece(3, 1) = Piece(PieceType::EMPTY);

    assert(board.hasAnyMoves(PieceType::WHITE) == true);
    assert(board.isValidMove(2, 2, 3, 1) == false);
    assert(board.isValidMove(2, 2, 4, 4) == true);
    cout << "testMandatoryCaptureBlocksNormalMove\n";
}

int main() {
    testdBackMove();
    testMoveForward();
    testCaptureMove();
    testInvalidCaptureBlocked();
    testPromotionToKing();
    testKingMoveAnyDirection();
    testDoubleCapturePossible();
    testKingCapture();
    testKingCannotCaptureMultiplePieces();
    testBackwardCaptureByMan();
    testMandatoryCaptureBlocksNormalMove();

    cout << "Все тесты пройдены!\n";
    return 0;
}