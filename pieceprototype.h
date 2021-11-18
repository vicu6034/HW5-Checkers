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
struct Position { int x, y; };

// PiecePrototype inherits both QObject & QGraphicsItem
class PiecePrototype : public QGraphicsObject //public QObject, public QGraphicsItem
{
    Q_OBJECT

protected:
    // each Piece has a Pos and color (bool)
    Position pos_;
    bool is_red_;
    // Player* owner_;

    // all Pieces have same size on screen
    static const int RADIUS = 20;

public:
    // constructors
    PiecePrototype(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {};

    // destructor
    virtual ~PiecePrototype() {}

    // reimplement copy constructor becuase QObject deletes it
    PiecePrototype(const PiecePrototype& p);

    // test method
    virtual void Test() {};
    // clone method
    virtual PiecePrototype *Clone() const = 0;

    // get / set
    Position get_position() const { return pos_; }
    bool get_is_red() const { return is_red_; }

    void set_position(Position pos) { pos_ = pos; }
    //void set_owner(Player* owner) { owner_ = owner; }

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override {
        Q_UNUSED(painter);
        Q_UNUSED(item);
        Q_UNUSED(widget);
    }

};

#endif // PIECEPROTOTYPE_H
