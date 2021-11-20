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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    // paramterized constructor
    Tile(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {}

    Position ConvertPosition() const { return Position{pos_.x*WIDTH, pos_.y*WIDTH}; }
    Position get_position() const { return pos_; }
    bool get_is_red() const { return is_red_; }
    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static const int WIDTH = 60;

signals:
    void gotSelected(Tile* t);

};

#endif // TILE_H
