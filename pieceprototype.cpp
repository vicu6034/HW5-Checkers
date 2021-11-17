#include "pieceprototype.h"

#include <QtWidgets>

// reimplement copy constructor becuase QObject deletes it
PiecePrototype::PiecePrototype(const PiecePrototype& old_p)
{
    prototype_name_ = old_p.get_name();
    pos_ = old_p.get_position();
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

// Set Piece visibility traits (color, size)
void PiecePrototype::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(QColor(255,0,0)));

    painter->drawEllipse(pos_.x, pos_.y, RADIUS, RADIUS);
    painter->setBrush(b);
}
