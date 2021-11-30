#include "regularpiece.h"

#include <QtWidgets>

// override paint to give Regular Pieces their own appearance
void RegularPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    Position pos = ConvertPosition();

    QBrush b = painter->brush();

    // add green highlight if piece is selected
    if (highlighted_) {
        painter->setBrush(QBrush(QColor(0, 255, 0)));
        painter->drawEllipse(
            pos.x - HIGHLIGHT_SIZE,
            pos.y - HIGHLIGHT_SIZE,
            RADIUS + (HIGHLIGHT_SIZE * 2),
            RADIUS + (HIGHLIGHT_SIZE * 2)
        );
    }

    // pieces are red or black
    if (is_red_) {
        painter->setBrush(QBrush(QColor(255,0,0)));
    } else {
        painter->setBrush(QBrush(QColor(0,0,0)));
        // give black pieces red outline/writing
        painter->setPen(Qt::red);
    }

    painter->drawEllipse(pos.x, pos.y, RADIUS, RADIUS);

    QImage king;
    king.load(":/images/king");
    king = king.scaled(RADIUS, RADIUS);

    painter->drawImage(pos.x, pos.y, king);

    painter->setBrush(b);
}


