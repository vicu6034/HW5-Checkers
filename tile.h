#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QGraphicsItem>

#include <pieceprototype.h>

class Tile : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    Position pos_;
    bool is_red_;

    static const int WIDTH = 30;

public:
    Tile() {};
    Tile(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {}

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // TILE_H
