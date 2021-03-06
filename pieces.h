/*
 * Pieces header
 * Implements different types of pieces
 * CSCI 3010 Homework 5
 * By: Vincent Curran & Philip Knott
*/

#ifndef PIECES_H
#define PIECES_H

#include <pieceprototype.h>

// Regular Piece inherits from base Piece
class RegularPiece : public PiecePrototype {
public:
    // constructor inherits from PiecePrototype
    RegularPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::RegularPiece; }

    // clone method (implicitly called by factory)
    PiecePrototype *Clone() const override {
       return new RegularPiece(*this);
    }

    // get all positions a regular piece can move to
    PositionVec GetPossibleMoves() const override;

    // override paint so Pieces have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
};

// King Piece inherits from base Piece
class KingPiece : public PiecePrototype {
public:
    // constructor inherits from PiecePrototype
    KingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::KingPiece; }
    // clone method (implicitly called by factory)
    PiecePrototype *Clone() const override {
        return new KingPiece(*this);
    }
    // get all positions a king can go to
    PositionVec GetPossibleMoves() const override;
    // override paint so Kings have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
};

// TripleKing inherits from base Piece
class TripleKingPiece : public PiecePrototype {
public:
    // constructor inherits from PiecePrototype
    TripleKingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::TripleKingPiece; }

    // clone method (implicitly called by factory)
    PiecePrototype *Clone() const override {
       return new TripleKingPiece(*this);
    }
    // get all positions a TK can go to
    PositionVec GetPossibleMoves() const override;
    // override paint so Pieces have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
};

#endif // PIECES_H
