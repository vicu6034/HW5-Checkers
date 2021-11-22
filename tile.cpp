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
    // tiles are red or black
    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
    }

    Position pos = ConvertPosition();

    painter->drawRect(pos.x, pos.y, WIDTH, WIDTH);
    painter->setBrush(b);
}

// emit gotselected signal for left clicks on black tiles
// slot for gotselected will check if the tile is a valid move for
// previously selected piece
void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !is_red_) {
        emit gotSelected(this);
    }
}
