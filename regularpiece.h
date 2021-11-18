#ifndef REGULARPIECE_H
#define REGULARPIECE_H

#include <pieceprototype.h>

class RegularPiece : public PiecePrototype
{

public:
    // constructor inherits from PiecePrototype
    RegularPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) {}

    // clone method (implicityly called by factory)
    PiecePrototype *Clone() const override {
       return new RegularPiece(*this);
    }

    // Test function to prove difference between Pieces
    void Test() override { qDebug() << "Regular"; }

    // override paint so Pieces have their own appearance
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // REGULARPIECE_H
