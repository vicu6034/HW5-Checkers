#include "tile.h"

#include <QtWidgets>

// set Tile position
QRectF Tile::boundingRect() const {
    auto pos = ConvertPosition();
    return QRectF(pos.x, pos.y, WIDTH, WIDTH);
}

// set Tile shape
QPainterPath Tile::shape() const {
    QPainterPath path;
    auto pos = ConvertPosition();
    path.addRect(pos.x, pos.y, WIDTH, WIDTH);
    return path;
}

// set Tile visibility traits (color, size)
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    auto b = painter->brush();
    // tiles are red or black
    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
    }

    auto pos = ConvertPosition();

    painter->drawRect(pos.x, pos.y, WIDTH, WIDTH);
    painter->setBrush(b);
}

// emit gotselected signal to gameboard for left clicks on black tiles
// gotselected handles trying to move a piece to the tile
void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // emit this tile to gameboard to try to move selected piece to it
        emit gotSelected(this);
    }
}
