#include "powerup.h"

#include <QtWidgets>

#include <pieceprototype.h>

// set piece position
QRectF PowerUp::boundingRect() const {
    Position pos = ConvertPosition();
    return QRectF(pos.x, pos.y, RADIUS, RADIUS);
}

// set Piece shape
QPainterPath PowerUp::shape() const {
    QPainterPath path;
    Position pos = ConvertPosition();
    path.addEllipse(pos.x, pos.y, RADIUS, RADIUS);
    return path;
}

// set Piece visibility traits (color, size)
void PowerUp::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();

    Position pos = ConvertPosition();

    // add piece image
    QString imagetype = is_addPiece_ ? "addpowerup" : "levelpowerup";
    QImage powerup(":/images/" + imagetype);
    powerup = powerup.scaled(RADIUS, RADIUS);
    painter->drawImage(pos.x, pos.y, powerup);

    painter->setBrush(b);
}

// return type of piece as a string
std::string PowerUp::get_type() {
    if (is_addPiece_) {
        return "add";
    } else {
        return "level";
    }
}
