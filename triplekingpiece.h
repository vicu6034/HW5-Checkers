#ifndef TRIPLEKINGPIECE_H
#define TRIPLEKINGPIECE_H

#include <pieceprototype.h>

class TripleKingPiece : public PiecePrototype
{

public:
    TripleKingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) {}
    //TripleKingPiece(std::string prototype_name)
    //    : PiecePrototype(prototype_name) {}

    PiecePrototype *Clone() const override {
        return new TripleKingPiece(*this);
    }

     void Test() override { qDebug() << "TripleKing" << pos_.x; }

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
};

#endif // TRIPLEKINGPIECE_H
