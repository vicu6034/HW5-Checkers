#include "pieceprototype.h"

#include <QtWidgets>

// copy old vals to new Piece (copy constructor)
PiecePrototype::PiecePrototype(const PiecePrototype& old_p) {
    highlighted_ = false;
    pos_ = old_p.get_position();
    is_red_ = old_p.get_is_red();
}

// helper for the pain override methods
void PiecePrototype::paintHelper(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget, QString piecetype) {
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

    // set image based on piece type and draw
    QImage checker(":/images/" + piecetype);
    checker = checker.scaled(RADIUS, RADIUS);
    painter->drawImage(pos.x, pos.y, checker);

    painter->setBrush(b);
}

// emit selected signal when Piece is left clicked
void PiecePrototype::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) emit gotSelected(this);
}

// set piece position
QRectF PiecePrototype::boundingRect() const {
    Position pos = ConvertPosition();
    return QRectF(pos.x, pos.y, RADIUS, RADIUS);
}

// set Piece shape
QPainterPath PiecePrototype::shape() const {
    QPainterPath path;
    Position pos = ConvertPosition();
    path.addEllipse(pos.x, pos.y, RADIUS, RADIUS);

    return path;
}
