#include "tile.h"

#include <QtWidgets>

// Set Tile position
QRectF Tile::boundingRect() const {
    return QRectF(pos_.x, pos_.y, WIDTH, WIDTH);
}

// Set Tile shape
QPainterPath Tile::shape() const {
    QPainterPath path;
    path.addRect(pos_.x, pos_.y, WIDTH, WIDTH);
    return path;
}

// Set Tile visibility traits (color, size)
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();

    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
    }

    painter->drawRect(pos_.x, pos_.y, WIDTH, WIDTH);
    painter->setBrush(b);
}
