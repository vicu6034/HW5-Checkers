#ifndef POWERUP_H
#define POWERUP_H

#include <pieceprototype.h>

class PowerUp : public QGraphicsObject //public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    Position pos_;
    bool is_addPiece_;

    static const int RADIUS = 20;

public:
    PowerUp(Position pos, bool is_addPiece) : pos_(pos), is_addPiece_(is_addPiece) {}

    bool is_add_piece() { return is_addPiece_; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // POWERUP_H
