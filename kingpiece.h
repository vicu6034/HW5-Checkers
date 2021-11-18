#ifndef KINGPIECE_H
#define KINGPIECE_H

#include <pieceprototype.h>

class KingPiece : public PiecePrototype
{

public:
    KingPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) {}
    //KingPiece(std::string prototype_name)
    //    : PiecePrototype(prototype_name) {}

    PiecePrototype *Clone() const override {
        return new KingPiece(*this);
    }

     void Test() override { qDebug() << "King" << pos_.x; }

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
};

#endif // KINGPIECE_H
