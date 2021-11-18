#ifndef POWERUP_H
#define POWERUP_H

#include <pieceprototype.h>

class PowerUp : public QGraphicsObject //public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    // each PowerUp has a position and type
    // addPiece : true : PowerUp adds piece to Player that collected it
    // addPiece : false : PowerUp upgrades Piece that lands on it
    Position pos_;
    bool is_addPiece_;

    // all PowerUps have same size on screen
    static const int RADIUS = 20;

public:
    // paramterized constructor with initialization list
    PowerUp(Position pos, bool is_addPiece) : pos_(pos), is_addPiece_(is_addPiece) {}

    // get what kind of PowerUp this is
    bool is_add_piece() { return is_addPiece_; }

    // necessarry Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // POWERUP_H
