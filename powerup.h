/*
 * Powerup header
 * Handles drawing powerups and keeping track of their type
 * CSCI 3010 Homework 5
 * By: Vincent Curran & Philip Knott
*/

#ifndef POWERUP_H
#define POWERUP_H

#include <QGraphicsObject>
#include <pieceprototype.h>

class PowerUp : public QGraphicsObject
{
    Q_OBJECT

private:
    // each PowerUp has a position and type
    Position pos_;
    // addPiece : true : PowerUp adds piece to Player that collected it
    // addPiece : false : PowerUp upgrades Piece that lands on it
    bool is_addPiece_;

    // all PowerUps have same size on screen
    static const int RADIUS = 50;

public:
    // paramterized constructor with initialization list
    PowerUp(Position pos, bool is_addPiece) : pos_(pos), is_addPiece_(is_addPiece) {}
    Position get_position() { return pos_; }
    std::string get_type();

    // convert pos_ to oncreen position
    Position ConvertPosition() const { return Position{pos_.x*60 + 5, pos_.y*60 + 5}; }

    // get what kind of PowerUp this is
    bool is_add_piece() { return is_addPiece_; }

    // necessarry Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // POWERUP_H
