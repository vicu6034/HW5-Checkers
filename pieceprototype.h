/*
 * PiecePrototype header
 * Implements methods and fields that all Pieces now
 * CSCI 3010 Homework 5
 * By: Vincent Curran & Philip Knott
*/

#ifndef PIECEPROTOTYPE_H
#define PIECEPROTOTYPE_H

#include <QGraphicsObject>
#include <QDebug>

// enum class to represent different Types of Pieces
enum class PieceType { RegularPiece = 0, KingPiece, TripleKingPiece };

// standard Position struct
struct Position {
    // a position has an x and y coordinate
    // top left is (0,0), bottom right is (n-1, n-1)
    int x, y;
    // override == to check for equality between positions
    friend bool operator==(Position p1, Position p2) {
        if ((p1.x == p2.x) && (p1.y == p2.y)) return true;
        else return false;
    }
};

// PiecePrototype inherits QGraphicsObject
class PiecePrototype : public QGraphicsObject
{
    Q_OBJECT

protected:
    // each Piece has a Pos and color (bool)
    Position pos_;
    bool is_red_;
    // keep type for easy access
    PieceType type_;
    bool highlighted_;

    // all Pieces have same size on screen
    static const int RADIUS = 50;
    static const int HIGHLIGHT_SIZE = 2;

    // handles what happens when the mouse is clicked
    // revive cell for left click, kill cell for right click
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void paintHelper(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget, QString piecetype);
    std::vector<Position> GetPossiblePos(int range) const;

public:
    // paramterized constructor
    PiecePrototype(Position pos, bool is_red) : pos_(pos), is_red_(is_red), highlighted_(false) {};

    // virtual destructor
    virtual ~PiecePrototype() {}

    // reimplement copy constructor becuase QObject deletes it
    PiecePrototype(const PiecePrototype& p);

    // convert a position from tile #s to screen position
    Position ConvertPosition() const { return Position{pos_.x*60 + 5, pos_.y*60 + 5}; }
    // clone method
    virtual PiecePrototype *Clone() const = 0;

    // getters and setters
    PieceType get_type() const { return type_; }
    Position get_position() const { return pos_; }
    bool get_is_red() const { return is_red_; }

    void set_position(Position pos) { pos_ = pos; }
    void set_highlighted(bool highlighted) { highlighted_ = highlighted; }

    virtual std::vector<Position> GetPossibleMoves() const = 0;

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override {
        Q_UNUSED(painter);
        Q_UNUSED(item);
        Q_UNUSED(widget);
    }

signals:
    // signal to emit when a piece is selected
    void gotSelected(PiecePrototype* p);

};

#endif // PIECEPROTOTYPE_H
