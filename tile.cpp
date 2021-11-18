#include "tile.h"

#include <QtWidgets>

// Set Tile position
QRectF Tile::boundingRect() const {
    Position pos = ConvertPosition();
    return QRectF(pos.x, pos.y, WIDTH, WIDTH);
}

// Set Tile shape
QPainterPath Tile::shape() const {
    QPainterPath path;
    Position pos = ConvertPosition();
    path.addRect(pos.x, pos.y, WIDTH, WIDTH);
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

    Position pos = ConvertPosition();

    painter->drawRect(pos.x, pos.y, WIDTH, WIDTH);
    painter->setBrush(b);
}
