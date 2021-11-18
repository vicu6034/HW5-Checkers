#include "regularpiece.h"

#include <QtWidgets>

// you can override the pieces paint methods to change their appearance

// Set Piece visibility traits (color, size)
void RegularPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();

    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
        painter->setPen(Qt::red);
    }

    painter->drawEllipse(pos_.x, pos_.y, RADIUS, RADIUS);

    painter->drawText(pos_.x+6, pos_.y+13, "R");

    painter->setBrush(b);
}


