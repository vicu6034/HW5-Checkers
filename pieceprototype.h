#ifndef PIECEPROTOTYPE_H
#define PIECEPROTOTYPE_H

#include <QGraphicsObject>
#include <QDebug>

// forward declaration of Player to solve cyclic dependency
// if we need *owner
// class Player;

// enum class to represent different Types of Pieces
enum class PieceType { RegularPiece = 0, KingPiece, TripleKingPiece };

// standard Position struct
struct Position {
    int x, y;

    friend bool operator==(Position p1, Position p2) {
        if ((p1.x == p2.x) && (p1.y == p2.y)) return true;
        else return false;
    }
};

// PiecePrototype inherits both QObject & QGraphicsItem
class PiecePrototype : public QGraphicsObject //public QObject, public QGraphicsItem
{
    Q_OBJECT

protected:
    // each Piece has a Pos and color (bool)
    Position pos_;
    bool is_red_;

    PieceType type_;
    // Player* owner_;

    // all Pieces have same size on screen
    static const int RADIUS = 50;

    // handles what happens when the mouse is clicked
    // revive cell for left click, kill cell for right click
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    // constructors
    PiecePrototype(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {};

    // destructor
    virtual ~PiecePrototype() {}

    // reimplement copy constructor becuase QObject deletes it
    PiecePrototype(const PiecePrototype& p);

    // convert a position from tile #s to screen position
    Position ConvertPosition() const { return Position{pos_.x*60 + 5, pos_.y*60 + 5}; }

    // test method
    virtual void Test() {};
    // clone method
    virtual PiecePrototype *Clone() const = 0;

    // get / set
    PieceType get_type() const { return type_; }
    Position get_position() const { return pos_; }
    bool get_is_red() const { return is_red_; }


    void set_position(Position pos) { pos_ = pos; }

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override {
        Q_UNUSED(painter);
        Q_UNUSED(item);
        Q_UNUSED(widget);
    }

signals:
    void gotSelected(PiecePrototype* p);

};

#endif // PIECEPROTOTYPE_H
