#include "pieceprototype.h"

#include <QtWidgets>

// copy old vals to new Piece
PiecePrototype::PiecePrototype(const PiecePrototype& old_p)
{
    pos_ = old_p.get_position();
    is_red_ = old_p.get_is_red();
}

// Set piece position
QRectF PiecePrototype::boundingRect() const {
    return QRectF(pos_.x, pos_.y, RADIUS, RADIUS);
}

// Set Piece shape
QPainterPath PiecePrototype::shape() const {
    QPainterPath path;
    path.addEllipse(pos_.x, pos_.y, RADIUS, RADIUS);
    return path;
}
