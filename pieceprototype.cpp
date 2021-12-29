#include "pieceprototype.h"

#include <QtWidgets>

// copy old vals to new Piece (copy constructor)
PiecePrototype::PiecePrototype(const PiecePrototype& old_p) {
    highlighted_ = false;
    pos_ = old_p.get_position();
    is_red_ = old_p.get_is_red();
}

// set piece position
QRectF PiecePrototype::boundingRect() const {
    auto pos = ConvertPosition();
    return QRectF(pos.x, pos.y, RADIUS, RADIUS);
}

// set Piece shape
QPainterPath PiecePrototype::shape() const {
    QPainterPath path;
    auto pos = ConvertPosition();
    path.addEllipse(pos.x, pos.y, RADIUS, RADIUS);

    return path;
}

// helper for the pain override methods
// @param QString type of piece to paint
void PiecePrototype::paintHelper(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget, QString piecetype) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    auto pos = ConvertPosition();

    auto b = painter->brush();

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

std::vector<Position> PiecePrototype::GetPossiblePos(int range) const {
    std::vector<Position> retV;
    // find all positions w/in 2
    for (int i = pos_.x-range; i <= pos_.x+range; i++) {
        for (int j = pos_.y-range; j <= pos_.y+range; j++) {
            // checking the tile is black & we're in the baords range
            if ((((i % 2 != 0) && (j % 2 != 0)) || ((i % 2 == 0) && (j % 2 == 0))) && (i >= 0) && (j >= 0) && (i <= 9) && (j <= 9)) {
                retV.push_back(Position{i,j});
            }
        }
    }
    return retV;
}

// emit selected signal when Piece is left clicked
// this sets the piece as 'selected' in the Game
void PiecePrototype::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) { emit gotSelected(this); }
}
