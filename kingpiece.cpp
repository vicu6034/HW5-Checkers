#include "kingpiece.h"

#include <QtWidgets>

// override paint to give Kings their own appearance
void KingPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();

    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
        painter->setPen(Qt::red);
    }

    Position pos = ConvertPosition();

    painter->drawEllipse(pos.x, pos.y, RADIUS, RADIUS);

    // Add king image
    QImage king(":/images/king");
    king = king.scaled(RADIUS, RADIUS);
    painter->drawImage(pos.x, pos.y, king);

    painter->setBrush(b);
}
