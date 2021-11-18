#ifndef TILE_H
#define TILE_H

#include <pieceprototype.h>

class Tile : public QGraphicsObject //public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    // each tile has a position and a color
    Position pos_;
    bool is_red_;

    // all tiles have the same size on screen


public:
    // paramterized constructor
    Tile(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {}

    Position ConvertPosition() const { return Position{pos_.x*WIDTH, pos_.y*WIDTH}; }

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static const int WIDTH = 60;

};

#endif // TILE_H
