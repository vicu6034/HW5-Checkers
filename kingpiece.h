#ifndef KINGPIECE_H
#define KINGPIECE_H

#include <pieceprototype.h>

class KingPiece : public PiecePrototype
{

public:
    // constructor inherits from PiecePrototype
    KingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) { type_ = PieceType::KingPiece; }

    // clone method (implicityly called by factory)
    PiecePrototype *Clone() const override {
        return new KingPiece(*this);
    }

     // override paint so Kings have their own appearance
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // KINGPIECE_H
