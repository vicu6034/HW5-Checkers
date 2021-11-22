#include "regularpiece.h"

#include <QtWidgets>

// override paint to give Regular Pieces their own appearance
void RegularPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();

    // pieces are red or black
    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
        // give black pieces red outline/writing
        painter->setPen(Qt::red);
    }

    Position pos = ConvertPosition();

    painter->drawEllipse(pos.x, pos.y, RADIUS, RADIUS);

    //painter->drawText(pos.x+20, pos.y+30, "R");

    painter->setBrush(b);
}


