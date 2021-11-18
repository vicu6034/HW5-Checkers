#ifndef REGULARPIECE_H
#define REGULARPIECE_H

#include <pieceprototype.h>
#include <QDebug>

class RegularPiece : public PiecePrototype
{

public:
    RegularPiece(Position pos, bool is_red) : PiecePrototype(pos, is_red) {}
    //RegularPiece(std::string prototype_name)
    //    : PiecePrototype(prototype_name) {}

    PiecePrototype *Clone() const override {
       return new RegularPiece(*this);
    }

    void Test() override { qDebug() << "Regular"; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
};

#endif // REGULARPIECE_H
