#ifndef REGULARPIECE_H
#define REGULARPIECE_H

#include <pieceprototype.h>

// Regular Piece inherits from base Piece
class RegularPiece : public PiecePrototype
{

public:
    // constructor inherits from PiecePrototype
    RegularPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::RegularPiece; }

    // clone method (implicityly called by factory)
    PiecePrototype *Clone() const override {
       return new RegularPiece(*this);
    }

    // override paint so Pieces have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // REGULARPIECE_H
