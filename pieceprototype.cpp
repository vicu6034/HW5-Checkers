#include "pieceprototype.h"

#include <QtWidgets>

// copy old vals to new Piece
PiecePrototype::PiecePrototype(const PiecePrototype& old_p)
{
    pos_ = old_p.get_position();
    is_red_ = old_p.get_is_red();
}

void PiecePrototype::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!is_selected_)
            emit gotSelected();
            is_selected_ = true;
    }
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
