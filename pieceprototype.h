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
    static const int RADIUS = 20;
    std::string prototype_name_;
    Position pos_;
    bool is_red_;
    // Player* owner_;

public:
    // constructors
    PiecePrototype(Position pos, bool is_red) : pos_(pos), is_red_(is_red) {};
    //PiecePrototype(std::string prototype_name)
    //    : prototype_name_(prototype_name) {}

    // destructor
    virtual ~PiecePrototype() {}

    // copy constructor
    PiecePrototype(const PiecePrototype& p);

    // test method
    virtual void Test() {};
    // clone method
    virtual PiecePrototype *Clone() const = 0;

    // get / set position
    // if we passed Pos in the constructor, it would have to be a dummy Pos, so theres no reason to
    // due to constructor being called through the Factory on its initialization we cant get Pos until
    // we need to make the Piece
    Position get_position() const { return pos_; }
    void set_position(Position pos) { pos_ = pos; }

    std::string get_name() const { return prototype_name_; }

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
