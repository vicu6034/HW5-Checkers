#include "powerup.h"

#include <QtWidgets>

// Set piece position
QRectF PowerUp::boundingRect() const {
    return QRectF(pos_.x, pos_.y, RADIUS, RADIUS);
}

// Set Piece shape
QPainterPath PowerUp::shape() const {
    QPainterPath path;
    path.addEllipse(pos_.x, pos_.y, RADIUS, RADIUS);
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

    painter->drawEllipse(pos_.x, pos_.y, RADIUS, RADIUS);

    painter->drawText(pos_.x+6, pos_.y+13, "P");

    painter->setBrush(b);
}
