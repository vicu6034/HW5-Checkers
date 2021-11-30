#include "pieceprototype.h"

#include <QtWidgets>

// copy old vals to new Piece
PiecePrototype::PiecePrototype(const PiecePrototype& old_p) {
    highlighted_ = false;
    pos_ = old_p.get_position();
    is_red_ = old_p.get_is_red();
}

// emit selected signal when Piece is left clicked
void PiecePrototype::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) emit gotSelected(this);
}

// Set piece position
QRectF PiecePrototype::boundingRect() const {
    Position pos = ConvertPosition();
    return QRectF(pos.x, pos.y, RADIUS, RADIUS);
}

// Set Piece shape
QPainterPath PiecePrototype::shape() const {
    QPainterPath path;
    Position pos = ConvertPosition();
    path.addEllipse(pos.x, pos.y, RADIUS, RADIUS);

    return path;
}
