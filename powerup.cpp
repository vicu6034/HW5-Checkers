#include "powerup.h"

#include <QtWidgets>

// Set piece position
QRectF PowerUp::boundingRect() const {
    Position pos = ConvertPosition();
    return QRectF(pos.x, pos.y, RADIUS, RADIUS);
}

// Set Piece shape
QPainterPath PowerUp::shape() const {
    QPainterPath path;
    Position pos = ConvertPosition();
    path.addEllipse(pos.x, pos.y, RADIUS, RADIUS);
    return path;
}

// Set Piece visibility traits (color, size)
void PowerUp::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();

    if (is_addPiece_) {
        painter->setBrush(QBrush(QColor(0,255,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,255)));
    }

    Position pos = ConvertPosition();

    painter->drawEllipse(pos.x, pos.y, RADIUS, RADIUS);

    painter->drawText(pos.x+6, pos.y+13, "P");

    painter->setBrush(b);
}
