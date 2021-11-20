#ifndef TRIPLEKINGPIECE_H
#define TRIPLEKINGPIECE_H

#include <pieceprototype.h>

class TripleKingPiece : public PiecePrototype
{

public:
    // constructor inherits from PiecePrototype
    TripleKingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::TripleKingPiece; }

    // clone method (implicityly called by factory)
    PiecePrototype *Clone() const override {
       return new TripleKingPiece(*this);
    }

    // Test function to prove difference between Pieces
    void Test() override { qDebug() << "TripleKing"; }

    // override paint so Pieces have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // TRIPLEKINGPIECE_H
