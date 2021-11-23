#ifndef TILE_H
#define TILE_H

#include <pieceprototype.h>

// tile inherits from QGraphicsObject
class Tile : public QGraphicsObject
{
    Q_OBJECT

private:
    // each tile has a position and a color
    Position pos_;
    bool is_red_;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    // paramterized constructor
    Tile(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {}

    // convert grid position to onscreen position
    Position ConvertPosition() const { return Position{pos_.x*WIDTH, pos_.y*WIDTH}; }

    // getters
    Position get_position() const { return pos_; }
    bool get_is_red() const { return is_red_; }

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    // all tiles have the same size on screen
    static const int WIDTH = 60;

signals:
    // custom signal to emit when a Tile is selected
    void gotSelected(Tile* t);

};

#endif // TILE_H
