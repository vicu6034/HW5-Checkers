#ifndef PIECEPROTOTYPE_H
#define PIECEPROTOTYPE_H

#include <QObject>
#include <QGraphicsItem>

enum class PieceType { RegularPiece = 0, KingPiece, TripleKingPiece };

struct Position { int x, y; };

class PiecePrototype : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    static const int RADIUS = 20;

protected:
    std::string prototype_name_;
    Position pos_;

public:
    // constructors
    PiecePrototype() {};
    PiecePrototype(std::string prototype_name)
        : prototype_name_(prototype_name) {}

    // destructor
    virtual ~PiecePrototype() {}

    // copy constructor
    PiecePrototype(const PiecePrototype& p);

    // clone method
    virtual PiecePrototype *Clone() const = 0;

    // get / set position
    // if we passed Pos in the constructor, it would have to be a dummy Pos, so theres no reason to
    // due to constructor being called through the Factory on its initialization we cant get Pos until
    // we need to make the Piece
    Position get_position() const { return pos_; }
    void set_position(Position pos) { pos_ = pos; }
    std::string get_name() const { return prototype_name_; }

    // necessary Qt bounding and drawing methods
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

};

#endif // PIECEPROTOTYPE_H
