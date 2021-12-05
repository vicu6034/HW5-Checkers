#include "triplekingpiece.h"

#include <QtWidgets>

// override paint to give TripleKing Pieces their own appearance
void TripleKingPiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
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

    // draw piece
    QString piecetype = is_red_ ? "redtriple" : "blacktriple";
    QImage checker(":/images/" + piecetype);
    checker = checker.scaled(RADIUS, RADIUS);
    painter->drawImage(pos.x, pos.y, checker);

    painter->setBrush(b);
}
