#ifndef TRIPLEKINGPIECE_H
#define TRIPLEKINGPIECE_H

#include <pieceprototype.h>

// TripleKing inherits from base Piece
class TripleKingPiece : public PiecePrototype
{

public:
    // constructor inherits from PiecePrototype
    TripleKingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::TripleKingPiece; }

    // clone method (implicityly called by factory)
    PiecePrototype *Clone() const override {
       return new TripleKingPiece(*this);
    }

    // override paint so Pieces have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // TRIPLEKINGPIECE_H
